# 🌟 GoldenStarOS

![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Architecture](https://img.shields.io/badge/arch-x86-blue)
![License](https://img.shields.io/badge/license-MIT-blueviolet)
![Stage](https://img.shields.io/badge/status-early--stage-yellow)

GoldenStarOS is a 32-bit hobbyist general operating system designed to be minimal and easily understood. It’s written from scratch in C and Assembly, targeting the x86 architecture.

At this stage, GoldenStarOS focuses on core kernel development — bootstrapping, memory management, paging, interrupts, and basic hardware interaction.

---

## 🎯 Purpose

GoldenStarOS exists to:

- 💡 Leaern systems programming through hands-on experimentation
- 🧱 Serve as a modular foundation for building OS features
- 🧠 To explore concepts like paging, interrupts, and memory management, and scheduling
- 🛠️ Provide a sandbox for hacking on low-level systems

---

## ✨ Features

> What’s already working

- 🔹 **Multiboot-compliant bootloader** (GRUB)
- 🔹 **Custom kernel** written in C and Assembly
- 🔹 **Global Descriptor Table (GDT)** and **Interrupt Descriptor Table (IDT)**
- 🔹 **Recursive Paging** and virtual memory setup
- 🔹 **Physical memory bitmap allocator**
- 🔹 **Kernel heap** (dynamic memory allocation)
- 🔹 **Basic terminal** with text rendering
- 🔹 **Interrupts:** Timer & PS/2 Keyboard
- 🔹 **ACPI: RSDP and MADT parsing**
- 🔹 **I/O APIC and Local APIC support**

---

## 🔮 Roadmap

> Features planned or in development

- [ ] 🧵 Preemptive multitasking
- [ ] 💬 Syscall interface
- [ ] 🧍 User-space program support
- [ ] 💾 Filesystem (FAT32 or custom)
- [ ] 🧠 Process scheduler
- [ ] ⌨️ Command-line shell

Once the basics are in place, I’ll decide on a unique direction to set GoldenStarOS apart — whether it’s experimentation with novel ideas, a specific use case, or just elegant simplicity.

---

## 🛠️ Getting Started

### 🔧 Requirements

- `i686-elf-gcc` (cross-compiler) I should probably include instructions on how to build this (or a convinient script)
- `nasm`
- `qemu`
- `make`
- Optional: `gdb` for debugging

### 🚀 Build & Run

```bash
./clean.sh; ./build.sh; ./qemu.sh

