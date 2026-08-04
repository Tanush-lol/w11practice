***practicing some wayland projects cuh i wnna build on wayland lol***

also im doing this on nixos where yea ill let u know in the first part lol
following tutorial - *https://youtu.be/v3eyPpWN4Ts* 

# Part 1
nga said we gotta get the waylandclient header file so we gotta do that except we are on nixos which is a bit harder

we need wayland-scanner which converts XML protocol file to da header 
the command is *wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml waylandclient.h*
this can be easily done by entering the nix-shell but we are not here to make the program reproducable rather unworkable on other machines **lol**

## error 1
[tanush@nixos:~/Projects/w11practice]$ wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml waylandclient.h
bash: wayland-scanner: command not found

- alr to get started we need to find wayland-scanner on the os  
[tanush@nixos:~]$ find /nix/store -type f -name wayland-scanner
output - /nix/store/7zcpywbkgw2gx9k0sy3285awbiwg51ci-wayland-scanner-1.25.0-bin/bin/wayland-scanner

wayland-scanner = /nix/store/7zcpywbkgw2gx9k0sy3285awbiwg51ci-wayland-scanner-1.25.0-bin/bin/wayland-scanner

/nix/store/7zcpywbkgw2gx9k0sy3285awbiwg51ci-wayland-scanner-1.25.0-bin/bin/wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml waylandclient.h

## error 2
[tanush@nixos:~/Projects/w11practice]$ /nix/store/7zcpywbkgw2gx9k0sy3285awbiwg51ci-wayland-scanner-1.25.0-bin/bin/wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml waylandclient.h
Could not open input file: No such file or directory

error directs to directory */usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml* 




