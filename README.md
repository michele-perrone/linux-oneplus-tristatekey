# OnePlus 6(T) tri-state key support in PostmarketOS
As the name suggest, the goal of this little project is to hook up the OnePlus6(T) tri-state key in PostmarketOS. This can be done in the userspace with the `gpiod` library.
## Installation and usage
Open your favorite terminal and type the following commands:
~~~
wget https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey/blob/main/perrone.michele%40outlook.com-6139e12f.rsa.pub
sudo cp perrone.michele@outlook.com-6139e12f.rsa.pub /etc/apk/keys/perrone.michele@outlook.com-6139e12f.rsa.pub
rm perrone.michele@outlook.com-6139e12f.rsa.pub
git clone https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey/tree/testing
abuild -r
sudo apk add tristatekey-daemon-0.2.0-r0.apk
sudo reboot now
~~~
Your OnePlus 6 will now reboot. Enjoy your tri-state key!

## Gimme the pins
The OnePlus 6 has three GPIO chips. Each of them has several (many, actually) lines. The three pins that correspond to the tri-state key belong to `gpiochip2` and are the following lines:
* 24 (bottom)
* 52 (middle)
* 126 (up)   

They are of the "active-high" type, which means that their value is `1` when triggered and `0` otherwise.
## Putting it together
1. The current value of the tri-state key has to be continuously polled by a daemon (inspiration [here](https://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux)), in order to abstract it with a debouce algorithm (examples [here](https://my.eng.utah.edu/~cs5780/debouncing.pdf)). It could be also possible to use interrupts instead of polling
2. The daemon should be an `rc-service` (follow [these](https://github.com/OpenRC/openrc/blob/master/service-script-guide.md) guidelines) since Alpine/PostmarketOS are OpenRC distributions and non Systemd ones
3. Based on the (debouced) value of the tri-state key, the daemon should switch between the three ring/vibrate/mute properties of the current desktop environment
