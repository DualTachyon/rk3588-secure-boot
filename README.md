# Support

* If you like my work, you can support me through https://ko-fi.com/DualTachyon

# Enabling Secure Boot on the RK3588 family of SoCs

The RK3588 family of SoCs have the ability to enable Secure Boot and ensuring that only approved bootloaders can run.
There is however very little public information in how to do so. Reverse engineering has revealed that 2 public released files have this ability:

- ["ramboot" bootloader](https://github.com/rockchip-linux/rkbin/blob/master/bin/rk35/rk3588_ramboot_v1.06.bin)

The ramboot loader requires sending it a bootloader that has been signed with the public key that you wish to use for Secure Boot.
Said bootloader must have been signed with the "sign_flag=0x20" in the rkbin/tools/settings.ini.

Problem: The loader has a bug that prevents loading a correct image. It expects to see an image of type RSAK and also RKSS, which is not possible.

- [OPTEE OS](https://github.com/rockchip-linux/rkbin/blob/master/bin/rk35/rk3588_bl32_v1.13.bin)

OPTEE_OS requires some specific SMC incantation and it is unknown to me who triggers this. Possibly u-boot, or some other 3rd party tools.
It also comes with extra side effects of not only enabling Secure Boot, but also fusing other bits onto the OTP. Some of which are related to the RockChip ToyBrick SBC. This may not be desirable.

Problem: I have not found an OS image that included OPTEE OS and I would rather not program OTPs that might be useful for other usecases than ToyBrick.

For these reasons, I thought it best to roll my own solution.

# Disclaimer

IF YOU DON'T FOLLOW STRICTLY THE INSTRUCTIONS DESCRIBED HERE, YOU RUN THE RISK OF BRICKING YOUR RK3588 DEVICE AND I WILL NOT BE HELD RESPONSIBLE FOR THIS OR ANY OTHER DAMAGE/CONSEQUENCES.

These instructions also only take care of the first off-chip bootloaders (e.g. miniloader / ramboot). Once you enable secure boot on your device, you're responsible for signing all further boot files to satisfy the OS/image you want to run.

Some details are deliberately omitted from this README to prevent accidental damage to your device. If you follow the instructions accurately, these details will be revealed during execution of each step. The source was written by design to not program any OTP until you follow certain steps and modify the source accordingly. Do not try to bypass these instructions and go on your own just to go quick.

# Requirements

- Your RK3588/RK3588S must expose UART2 onto easy access pins. UART2 is required to enable Secure Boot and OTP programming will not work without it.
  - Both Orange PI 5B and Orange PI 5+ fullfill this requirement. Your mileage may vary with other SBCs.

- clang is the expected compiler. The makefile expects the aarch64 sysroot at /usr/aarch64-linux-gnu/include. If necessary, modify the Makefile to suit your needs.

- If you prefer, you can use aarch64-linux-gnu-gcc by changing USE_CLANG=1 to 0 in the Makefile.

- You need to prepare your environment in the following way:

```
$ mkdir secboot
$ cd secboot
$ git clone https://github.com/DualTachyon/rk3588-secureboot
$ git clone https://github.com/DualTachyon/rk3588-tools
$ git clone https://github.com/rockchip-linux/rkbin
$ git clone https://github.com/rockchip-linux/rkdeveloptool
```

# Generating the OTP data and the bootloader to program them

The steps to enable Secure Boot on a RK3588 chip are below. Each step assume you start from "secboot"'s parent folder to avoid confusion.

- Generate your own signing key

```
$ cd secboot
$ mkdir keys
$ cd keys
$ openssl genrsa -f4 -out private_key.pem 2048 # You can also use 4096 if preferred
$ openssl pkey -in private_key.pem -pubout -out public_key.pem
```

You can also use the RockChip [signing tool](https://github.com/rockchip-linux/rkbin/blob/master/tools/rk_sign_tool) for this:

```
$ cd secboot
$ mkdir keys
$ cd keys
$ ../rkbin/tools/rk_sign_tool kk --bits=2048 --out . # You can also use 4096 if preferred
```
This will generate both private_key.pem and public_key.pem.

- Sign any binary. You can use one from the [rkbin repository](https://github.com/rockchip-linux/rkbin/).

```
$ cd secboot
$ cd rkbin
$ ./tools/rk_sign_tool cc --chip 3588
$ ./tools/rk_sign_tool ss --flag=0x20
$ ./tools/boot_merger RKBOOT/RK3588MINIALL_RAMBOOT.ini

This generates an unsigned binary named rk3588_ramboot_loader_v1.12.106.bin. The filename may be slightly different depending on RockChip updates.

$ ./tools/rk_sign_tool lk --key ../keys/private_key.pem --pubkey ../keys/public_key.pem
$ ./tools/rk_sign_tool sl --loader rk3588_ramboot_loader_v1.12.106.bin

This will complain twice about "failed to get key = sign_algo". You can safely ignore this message.
```

- Generate OTP data for the current bootloader

```
$ cd secboot
$ cd rk3588-tools
$ make
$ ./rk-genotp -i ../rkbin/rk3588_ramboot_loader_v1.12.106.bin
```

This will output some instructions on the terminal that you have to follow strictly.
Here's a sample output:

```
$ ./rk-genotp -i ../rkbin/rk3588_ramboot_loader_v1.12.106.bin
RK3588 Secure Boot OTP generator. Copyright 2024 Dual Tachyon

Paste the following line inside BootKeyHash[]:
        0xC794BE1B, 0xEB18A9E8, 0x81A4D7A5, 0x0C37DF34, 0x42034A5F, 0x196FF4C2, 0xE8323F39, 0xB3965B40,

Paste the folliwing inside BootKeyHashCheck: 0xA1D7C586
```

- Build the bootloader. Take the output from the previous step and add them to the main.c file. Search for "BootKeyHash" and "BootKeyHashCheck" and edit as instructed.

```
$ cd secboot
$ cd rk3588-secure-boot
$ <your favourite editor> main.c
$ make
```

The output should look something like this:

```
aarch64-linux-gnu-ld entry.o main.o exceptions.o libc.o otp.o printf.o putchar.o swd.o uart.o vbar.o -o enable-sb.elf -T linker.ld
aarch64-linux-gnu-objcopy -O binary enable-sb.elf enable-sb.bin
```

- Pack the bootloader for USB

```
$ cd secboot
$ cd rk3588-secure-boot
$ ../rk3588-tools/rk-packsign --usb -i enable-sb.bin -o enable-sb.usb.bin
```

- Run the bootloader. Ensure your RK3588 device is in "MaskROM" boot mode, otherwise it will not work.

```
$ cd secboot
$ cd rkdeveloptool
$ make
$ ./rkdeveloptool db ../rk3588-secure-boot/enable-sb.usb.bin
```

You may see that rkdeveloptool will print an error and that the download failed. For this binary only, this is normal and you should look at the UART to determine whether you were successful or not.

- At this point, you should pay close attention to the UART output and follow any instructions given in it.

- Secure boot is enabled in 3 stages:
  - Boot the loader the first time. Various checks will be performed and you will be given instructions to continue.
  - After following the instructions on the UART, boot the loader again and the key hash will be programmed.
  - If the output of the second run is successful, reset the device and run it a third time to enable secure boot.
  - Once again, follow the instructions accurately.

- If you made it this far, you want to test Secure Boot is working as expected:

```
$ cd secboot
$ cd rk3588-secure-boot
$ ../rk3588-tools/rk-packsign --usb -i enable-sb.bin -o enable-sb.usb.bin --key ../keys/private_key.pem
```

If Secure Boot was enabled successfully, you should see the following output:

```
EL3 start!
Boot key hash has been programmed successfully!
Secure Boot is already enabled!
```

- You can now use the keys you generated to sign other files using RockChip's rk_sign_tool.

# License

Copyright 2024 Dual Tachyon
https://github.com/DualTachyon

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

