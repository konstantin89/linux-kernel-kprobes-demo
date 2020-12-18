# kprobes based linux kernel process monitor

## Brief

Implementation of basic kprobes demo kernel module.  
The  module places kprobes probe on do_exit.   
On each exited process, the module will print  
a log with the process name and exit code.

Example output: 
``` text
[ 3692.905970] handler_pre : handler_pre: name [sudo], pid [6250], exit_code [0] 
[ 3692.906176] handler_pre : handler_pre: name [make], pid [6249], exit_code [0] 
[ 3696.337482] handler_pre : handler_pre: name [dircolors], pid [6255], exit_code [0] 
[ 3696.608100] handler_pre : handler_pre: name [systemd-udevd], pid [6252], exit_code [0] 
[ 3707.276686] handler_pre : handler_pre: name [kworker/dying], pid [2688], exit_code [0] 
[ 3711.833541] handler_pre : handler_pre: name [pool], pid [6253], exit_code [0] 
[ 3711.983803] handler_pre : handler_pre: name [Backgro~Pool #3], pid [3513], exit_code [0] 
[ 3712.709814] handler_pre : handler_pre: name [git], pid [6258], exit_code [0] 

```

## Usage

In order to install the kenrel module use the following steps:

Step 1 - compile the kernel module object file.  
```
make
```

Step 2 - Install the kernel module into the kernel.  
file.  
```
make install
```

Step 3 - Observe the logs printed by the module.  

file.  
```
sudo dmesg -w
```

To uninstall the module please use the following command:

``` bash
make uninstall
```


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