#ifndef SVM_RUNTIME_SPEC_H
#define SVM_RUNTIME_SPEC_H

/*
 * SVM Runtime Specification
 * -------------------------
 * Canonical runtime interface for CLI compliance.
 *
 * Every runtime must implement these functions using SVM_Value inputs and outputs.
 * This header is NOT meant to be included in C source code.
 */

#error "SVM_RUNTIME_SPEC_H is a specification header and must not be included in source code."

/* --------------------------------------------------------------------------
 * Generic VM Value
 * --------------------------------------------------------------------------
 *
 * Only three types are valid:
 * - number: numeric values (double)
 * - string: null-terminated UTF-8 strings
 * - list: dynamic array of SVM_Value slots
 */
/*
typedef union SVM_Value {
    double       number;
    const char*  string;
    void*        list;   // opaque pointer to internal list object
} SVM_Value;
*/

/* --------------------------------------------------------------------------
 * Typedefs for clarity/error handling
 * --------------------------------------------------------------------------
 */
/*
typedef void* SVM_List;    // opaque pointer to a list object (scope or value container)
typedef void* SVM_Opcode;  // opaque pointer to a single instruction
*/

/* --------------------------------------------------------------------------
 * Lists as Scopes and Code
 * --------------------------------------------------------------------------
 *
 * - Each list is a lexical scope and contains slots of SVM_Value or opcodes.
 * - Slots can hold:
 *     - number
 *     - string
 *     - list (nested scope)
 *     - opcode (instruction)
 * - Nested lists create nested scopes.
 * - Variable lookup is upward-only.
 * - Each scope is assigned a numeric ID during compilation (top-down order).
 */

/* --------------------------------------------------------------------------
 * Internal Representation of Slots in Memory
 * --------------------------------------------------------------------------
 *
 * Each slot in a list/scope is stored in memory as:
 *
 *   [type][data]
 *
 * Types:
 *   __VALUE__  -> actual data (number, string, list)
 *   __OPCODE__ -> instruction (opcode_id)
 *
 * Examples:
 *
 * Number slot:
 *   [__VALUE__][42.0]
 *
 * String slot:
 *   [__VALUE__]["Hello, world!"]
 *
 * List slot:
 *   [__VALUE__][pointer to nested SVM_List][length]
 *
 * Opcode slot:
 *   [__OPCODE__][opcode_id]
 *
 * Execution:
 * - The VM iterates over slots in a list.
 * - __VALUE__ slots are treated as data.
 * - __OPCODE__ slots are executed by the runtime.
 * - Nested lists may contain both __VALUE__ and __OPCODE__.
 * - Jump instructions always target the first slot of a nested list (start of scope).
 */

/* --------------------------------------------------------------------------
 * Full Example Program Memory Layout and Execution Diagram
 * --------------------------------------------------------------------------
 *
 * Top-Level Scope (ID 0) - Chunk Allocated
 * +-------------------------------------------------------------+
 * | Slot 0: [__OPCODE__][SET]        ; x = 0                    |
 * | Slot 1: [__VALUE__][0.0]                                    |
 * | Slot 2: [__OPCODE__][SET]        ; max = 5                  |
 * | Slot 3: [__VALUE__][5.0]                                    |
 * | Slot 4: [__OPCODE__][PRINT]      ; "Counting up:"           |
 * | Slot 5: [__VALUE__]["Counting up:"]                         |
 * | Slot 6: [__VALUE__][pointer to Scope ID 1][len=3] ; loop    |
 * | Slot 7: [__OPCODE__][PRINT]      ; "Done!"                  |
 * | Slot 8: [__VALUE__]["Done!"]                                |
 * +-------------------------------------------------------------+
 *
 * Nested Loop Scope (ID 1) inside Slot 6
 * +-------------------------------------------------------------+
 * | Slot 0: [__OPCODE__][PRINT]        ; print x                |
 * | Slot 1: [__VALUE__][x]                                      |
 * | Slot 2: [__OPCODE__][ADD]          ; x + 1                  |
 * | Slot 3: [__VALUE__][1.0]                                    |
 * | Slot 4: [__OPCODE__][SET]          ; update x               |
 * | Slot 5: [__VALUE__][x]                                      |
 * | Slot 6: [__OPCODE__][IF_LT_JUMP]    ; if x < max jump       |
 * | Slot 7: [__VALUE__][scope_ID_1]                             |
 * +-------------------------------------------------------------+
 *
 * Macro Expansion Example
 * +-------------------------------------------------------------+
 * | Slot 0: [__OPCODE__][SYSCALL]      ; macro: log value       |
 * | Slot 1: [__VALUE__][id_macro_log]                           |
 * | Slot 2: [__VALUE__][x]                                      |
 * +-------------------------------------------------------------+
 *
 * Math Operation Example
 * +-------------------------------------------------------------+
 * | Slot 0: [__OPCODE__][ADD]          ; variadic math          |
 * | Slot 1: [__VALUE__][x]                                      |
 * | Slot 2: [__VALUE__][1.0]                                    |
 * +-------------------------------------------------------------+
 *
 * Notes:
 * - __OPCODE__ slots are executed sequentially.
 * - __VALUE__ slots provide arguments, nested lists/scopes, or constants.
 * - Loops and conditional jumps reference numeric scope IDs.
 * - Macros expand into opcode/value sequences at compile time.
 * - All scopes are chunk-allocated; top-level freed at program exit.
 * - Variable lookup is upward-only: nested scopes can read parents.
 * - Bytecode can be serialized as:
 *     [type][data] sequences in memory.
 */

/* --------------------------------------------------------------------------
 * Text (.svmp) and Bytecode (.svmb) Representations
 * --------------------------------------------------------------------------
 *
 * Text (.svmp)                     | Bytecode (.svmb)
 * --------------------------------------------------------------------------
 * (set x 42)                       | [__OPCODE__][SET] [__VALUE__][42.0]
 * (set max 10)                     | [__OPCODE__][SET] [__VALUE__][10.0]
 * (print "Hello")                  | [__OPCODE__][PRINT] [__VALUE__]["Hello"]
 * (add x 1)                        | [__OPCODE__][ADD] [__VALUE__][1.0]
 * (if (< x max) (jump loop))       | [__OPCODE__][IF_LT_JUMP] [__VALUE__][target_scope_ID]
 * (macro ...)                      | Compiled inline into opcode + value sequence
 * (syscall id args...)             | [__OPCODE__][SYSCALL] [__VALUE__][id] [__VALUE__][args...]
 * (list ...)                       | [__VALUE__][pointer_to_list][length]
 *
 */

/* --------------------------------------------------------------------------
 * Builtin Functions / Runtime API
 * --------------------------------------------------------------------------
 */
/*
SVM_Value length(SVM_List list);                  // return number of slots
SVM_Value get(SVM_List list, SVM_Value index);    // access slot by index
void       set(SVM_List list, SVM_Value index, SVM_Value val); // assign slot
*/

/* --------------------------------------------------------------------------
 * Basic I/O
 * --------------------------------------------------------------------------
 */
/*
void      svm_print(SVM_Value val);           
void      svm_println(SVM_Value val);         
SVM_Value svm_read();                         
void      svm_write_file(const char* filename, SVM_Value val);
SVM_Value svm_read_file(const char* filename);
*/

/* --------------------------------------------------------------------------
 * Memory / Object Management
 * --------------------------------------------------------------------------
 */
/*
SVM_List  svm_alloc(SVM_Value type, SVM_Value count); 
void      svm_free(SVM_Value val);                  
SVM_List  svm_realloc(SVM_List list, SVM_Value new_count);
*/

/* --------------------------------------------------------------------------
 * System / Environment
 * --------------------------------------------------------------------------
 */
/*
void      svm_exit(SVM_Value code);      
SVM_Value svm_time();                     
*/

/* --------------------------------------------------------------------------
 * Math / Variadic Operations
 * --------------------------------------------------------------------------
 */
/*
SVM_Value svm_add(int argc, ...);  
SVM_Value svm_sub(int argc, ...);
SVM_Value svm_mul(int argc, ...);
SVM_Value svm_div(int argc, ...);
*/

/* --------------------------------------------------------------------------
 * Custom / Optional Runtime Extensions
 * --------------------------------------------------------------------------
 */
/*
SVM_Value svm_syscall(SVM_Value id, int argc, ...);
*/

/* --------------------------------------------------------------------------
 * Notes:
 * - Only number, string, and list are valid SVM_Value types.
 * - Lists/scopes are chunk-allocated and contain both __VALUE__ and __OPCODE__ slots.
 * - Variable lookup is upward-only.
 * - Jumps target numeric scope IDs and always land at the start of a scope.
 * - Entire program is loaded in memory for fast execution and macro expansion.
 * - Header is purely documentation/specification and must not be included.
 */

#endif /* SVM_RUNTIME_SPEC_H */
