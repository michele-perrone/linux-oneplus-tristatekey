# OnePlus tri-state key support on Linux distributions
As the name suggest, the goal of this little project is to hook up the tri-state key for OnePlus phone in Linux distributions. This is accomplished in the userspace thanks the `gpiod` library and a d-bus interface.
## Installation (currently for OnePlus 6 on PostmarketOS only!)
Open your favorite terminal and type the following commands:
~~~
wget https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey/releases/tag/0.2.0-r0
sudo cp perrone.michele@outlook.com-6139e12f.rsa.pub /etc/apk/keys/perrone.michele@outlook.com-6139e12f.rsa.pub
sudo apk add tristatekey-daemon-0.2.0-r0.apk
sudo reboot now
~~~
Your OnePlus 6 will now reboot. Enjoy your tri-state key!
## Building from source (currently for OnePlus 6 on PostmarketOS only!)
From your terminal:
~~~
git clone https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey
abuild
~~~
Keep in mind that if you're running `abuild` for the first time, you'll be asked to create your public-private key pair.   
The default directory for the compiled package is in `~/packages/projects/aarch64/`.
## Gimme the pins (OnePlus 6)
The OnePlus 6 has three GPIO chips. Each of them has several (many, actually) lines. The three pins that correspond to the tri-state key belong to `gpiochip2` and are the following lines:
* 24 (bottom)
* 52 (middle)
* 126 (up)   

They are of the "active-high" type, which means that their value is `1` when triggered and `0` otherwise.
## Putting it all together
1. At startup, two daemons wake up: the root daemon and the user daemon. The root daemon is started with OpenRC/Systemd, while the user daemon has a `.desktop` file and starts when the user logs into their session
2. The current value of the tri-state key is continuously continuously polled by the root daemon with a rate of `POLL_RATE_MS`. It could be also possible to use interrupts instead of polling. The root daemon encodes the value into a single integer and forwards it into a pipe
3. The user daemon reads the value from the pipe and sets the ring/vibrate/mute property through the sessio d-dbus based on its current value. We don't have to care about polling/interrupts here because of the blocking nature of the pipe

## TODO (in no particular order, but has to be done)
* [ ] Read the GPIO chip and lines from a configuration file matching `/etc/conf.d/ttk.*.conf`
* [ ] Remove the PostmarketOS-related stuff from this repository. Distro-specific things should be in their repositories (for PMOS: pmaports)
* [ ] In the root daemon, try using interrupts instead of polling
* [ ] Prepare systemd scripts
* [ ] Test on other distributions, eg. Mobian
