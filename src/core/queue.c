// #define QUEUE_TYPE u4

// #include "core/core.h"
// #include "core/result.h"

// #define mGenericQueue(T,lowerCaseT) \
//     T##QueueNode \
//     { \
//         T##QueueNode *next; \
//         T##QueueNode *prev; \
//         index_t index; \
//         T data; \
//     }; \
//     T##Queue \
//     { \
//         T##QueueNode *head; \
//         T##QueueNode *tail; \
//         count_t length; \
//     }; \
//     struct Result lowerCaseT##QueueCreate(T##Queue *dest) \
//     { \
//         dest->head = NULL; \
//         dest->tail = NULL; \
//         dest->length = 0; \
//         return ok(); \
//     } \
//     struct Result lowerCaseT##queueInsert(T##Queue *self, T data) \
//     { \
//         if (self->head == NULL) \
//         { \
//             self->head = self->tail = malloc(sizeof(T##QueueNode)); \
//             self->head->next = NULL; \
//             self->head->prev = NULL; \
//             self->head->data = data; \
//         } \
//         else \
//         { \
//             T##QueueNode *node = malloc(sizeof(T##QueueNode)); \
//             node->next = self->head; \
//             node->prev = NULL; \
//             self->head->prev = node; \
//             self->head = node; \
//             node->data = data; \
//         } \
//         self->length++; \
//         return ok(); \
//     } \
//     struct Result lowerCaseT##queueRemove(T##Queue *self, T data) \
//     { \
//         if (self->head == NULL) \
//         { \
//             return mErr("Queue is empty"); \
//         } \
//         if (self->head->data == data) \
//         { \
//             self->head = self->head->next; \
//             if (self->head != NULL) \
//             { \
//                 self->head->prev = NULL; \
//             } \
//             else \
//             { \
//                 self->tail = NULL; \
//             } \
//             free(node); \
//             self->length--; \
//             return ok(); \
//         } \
//         else \
//         { \
//             T##QueueNode *node = self->head; \
//             while (node->next != NULL) \
//             { \
//                 if (node->next->data == data) \
//                 { \
//                     node->next->prev = node->prev; \
//                     node->prev->next = node->next; \
//                     free(node); \
//                     self->length--; \
//                     return ok(); \
//                 } \
//                 node = node->next; \
//             } \
//             return mErr("Data not found"); \
//         } \      
//     } 

// #if defined(QUEUE_TYPE) 
    
// #endif
