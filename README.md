SVM — ("A Virtual Machine that runs on lists.")
===============================================

SVM is an open-source **S-expression Virtual Machine** written in **C**, designed to execute programs represented as S-expressions efficiently and portably. Inspired by the elegance of Lisp, SVM treats **code as data** and provides a minimalist, high-performance runtime for experimenting with Lisp-like languages or building new DSLs.

Projects
--------

SVM consists of two main projects:

1.  **Runtime (`libsvm`)** – A shared library implementing the S-expression VM. Can be embedded in other projects or linked by the CLI to produce native executables.
2.  **CLI (`svm`)** – A command-line interface that can:
    *   Execute `.svmp` human-readable S-expression programs (internally compiled to bytecode)
    *   Emit `.svmb` compiled bytecode
    *   Run `.svmb` bytecode directly
    *   Compile programs into standalone native executables (from `.svmp` or `.svmb`)
    *   Run programs on custom runtimes (e.g., redirect I/O to a file or custom backend)

File Extensions
---------------

*   **`.svmp`** – Human-readable SVM program (“assembly” for the VM). When executed, the VM internally compiles it into `.svmb` bytecode.
*   **`.svmb`** – Compiled SVM bytecode. Can be emitted ahead-of-time and executed by any SVM runtime, making it portable across environments.

> Think of `.svmp` as the “assembly” and `.svmb` as the “machine code.” The VM always executes bytecode internally, whether compiled ahead-of-time or just-in-time.

Features
--------

*   **S-expression based:** Programs are represented as lists, making code introspection and metaprogramming natural.
*   **Dual usage:** Use the runtime as a library or compile S-expressions into native binaries with the CLI.
*   **Bytecode support:** Emit `.svmb` files and execute them directly for faster performance and portability.
*   **Native compilation:** Compile `.svmp` or `.svmb` to standalone native executables.
*   **Custom runtimes:** The CLI/runtime can run programs on user-defined runtimes. For example, all I/O operations can be redirected to a file or another backend.
*   **Portable & self-contained:** Written in pure C using only the standard library — no external dependencies.
*   **Lightweight & extensible:** Minimal footprint, easy to extend with custom primitives.
*   **Playful but serious:** Runs on parentheses — literally and figuratively.

Getting Started
---------------

Clone the repository:

    git clone https://github.com/A-Boring-Square/svm.git
    cd svm
    

Build both runtime and CLI with Meson:

    meson setup build
    meson compile -C build
    

### Running Programs

Execute a `.svmp` program (internally compiled to bytecode at runtime):

    ./build/svm your_program.svmp
    

Emit portable `.svmb` bytecode from `.svmp` for distribution to other runtimes:

    ./build/svm --emit-bytecode your_program.svmp -o your_program.svmb
    

Run compiled `.svmb` bytecode directly on any SVM runtime:

    ./build/svm your_program.svmb
    

Compile `.svmp` or `.svmb` to a native executable:

    ./build/svm --compile your_program.svmp -o myprogram
    # or from bytecode
    ./build/svm --compile your_program.svmb -o myprogram
    ./myprogram
    

Run programs with a custom runtime (example: redirect I/O to a file):

    ./build/svm --runtime myfileio_runtime your_program.svmp
    

Example
-------

    ; Example S-expression program
    (print (add 1 2 3))
    

Contributing
------------

SVM is open-source — contributions are welcome! Whether it’s improving the runtime, adding new CLI features, or enhancing documentation, your help makes SVM stronger.

> “All you need is a list.”

Workflow Diagram
----------------

    
          .svmp (human-readable source)
                    |
                    | --[internal JIT compilation]--> .svmb (bytecode)
                    |
              +-------------------------------+
              | CLI / Runtime                 |
              |                               |
              | Execute .svmp                 | --> internally compiles to .svmb and runs
              | Execute .svmb                 | --> runs bytecode directly (portable)
              | Compile to exe                | --> produces native executable from .svmp or .svmb
              | Custom runtime support        | --> e.g., redirect I/O to files or other backends
              +-------------------------------+
                    |
                    |---> .svmb can be sent to other SVM runtimes
