#include "swapchain.h"
#include "pico/types.h"
#include <stdlib.h>

/**
 * @brief Initialize the chain before use
 *
 * Must be called before calling any other swapchain_*()` methods.
 *
 * @param chain Pointer to an initialized swapchain in memory
 * @param buffer_size Size of buffer in each node
 * @param nodes Number of swappable nodes
 */
void swapchain_init(Swapchain *chain, uint buffer_size, uint nodes) {
    uint i;
    SwapchainNode *current, *next;

    void *p_mem = malloc(nodes * (sizeof(SwapchainNode) + buffer_size));

    chain->cursor = p_mem;
    chain->buffer_size = buffer_size;
    chain->nodes = nodes;

    void *p_buffer_mem = p_mem + (nodes * sizeof(SwapchainNode));

    for (i = 0; i < nodes; i++) {
        current = &chain->cursor[i];
        next = &chain->cursor[(i + 1) % nodes];

        current->borrowed = false;
        current->buffer = p_buffer_mem;
        current->next = next;
        next->prev = current;

        p_buffer_mem += buffer_size;
    }
}

/**
 * @brief Get the size of each buffer contained inside each node
 *
 * @param chain
 * @return uint
 */
uint swapchain_get_buffer_size(const Swapchain *chain) {
    return chain->buffer_size;
}

/**
 * @brief Get the number of nodes in the swapchain
 *
 * @param chain
 * @return uint
 */
uint swapchain_get_node_count(const Swapchain *chain) { return chain->nodes; }

/**
 * @brief Borrow a Node for writing to it
 *
 *  Borrows the node next to the cursor.
 *  Keeps cursor unchanged.
 *
 * @param chain
 * @return Node*
 */
SwapchainNode *swapchain_try_borrow_for_write(Swapchain *chain) {
    SwapchainNode *taken;

    if ((taken = chain->cursor->next) == chain->cursor)
        return NULL;

    taken->prev->next = taken->next;
    taken->next->prev = taken->prev;

    return taken;
}

/**
 * @brief Return a Node after writing
 *
 * Inserts the node next to the cursor.
 * Moves the cursor to the inserted node.
 *
 * @param chain
 * @param node
 */
void swapchain_return_after_write(Swapchain *chain, SwapchainNode *node) {
    SwapchainNode *prev = chain->cursor;
    SwapchainNode *next = chain->cursor->next;

    // Insert the node
    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;

    // Change the cursor
    chain->cursor = node;
}

/**
 * @brief Borrow a Node for reading
 *
 * Borrows the node at the cursor.
 * Moves cursor to the next node.
 *
 * @param chain
 * @return Node*
 */
SwapchainNode *swapchain_try_borrow_for_read(Swapchain *chain) {
    SwapchainNode *taken;

    if ((taken = chain->cursor) == chain->cursor->next)
        return NULL;

    taken->prev->next = taken->next;
    taken->next->prev = taken->prev;
    chain->cursor = taken->next;

    return taken;
}

/**
 * @brief Return a Node after reading
 *
 * Inserts the node next to the cursor.
 * Keeps cursor unchanged.
 *
 * @param chain
 * @param node
 */
void swapchain_return_after_read(Swapchain *chain, SwapchainNode *node) {
    SwapchainNode *prev = chain->cursor;
    SwapchainNode *next = chain->cursor->next;

    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;
}

/**
 * @brief Returns the raw pointer in memory for the audio buffer
 *
 * @param frame The pointer to the initialized audio frame in memory
 * @return void* The pointer to the start of the buffer
 */
void *swapchain_node_get_buffer_ptr(const SwapchainNode *node) {
    return node->buffer;
}

/**
 * @brief Destroy the chain
 *
 * Renders the chain useless after this call. Any
 * further calls to swapchain_*() methods after this call is undefined
 * behaviour.
 *
 * @param chain Pointer to the chain
 */
void swapchain_deinit(Swapchain *chain) {
    if (!chain)
        return;

    // Unified memory model
    free(chain->cursor);
}