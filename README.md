# kprobes based linux kernel process monitor

## Brief



## Links 

[kprobes documentation](https://www.kernel.org/doc/Documentation/kprobes.txt)

[x64 registers struct](https://elixir.bootlin.com/linux/latest/source/arch/x86/include/uapi/asm/ptrace.h#L44)

[_do_fork source](https://elixir.bootlin.com/linux/v4.5/source/kernel/fork.c#L1691)

[_do_fork guide](https://s-matyukevich.github.io/raspberry-pi-os/docs/lesson04/linux/fork.html)

[x64 regirster struct](https://elixir.bootlin.com/linux/latest/source/arch/x86/include/uapi/asm/ptrace.h#L44)

[x64 gcc ABI](https://www.systutorials.com/x86-64-calling-convention-by-gcc/)

</br>

## Kprobes notes

Probe handlers are run with preemption disabled or interrupt disabled,    
which depends on the architecture and optimization state.  
In any case, your handler should not yield the CPU (e.g., by attempting  
to acquire a semaphore, or waiting I/O).    