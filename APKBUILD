# Contributor: Michele Perrone <perrone.michele@outlook.com>
# Maintainer: Michele Perrone <perrone.michele@outlook.com>
pkgname=tristatekey-daemon
pkgver=0.1.0
pkgrel=0
pkgdesc="A daemon that hooks up the tri-state key on the OnePlus6(T)"
url="https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey"
arch="aarch64"
license="GPL"
install="$pkgname.pre-install"
source="build-tristatekey-daemon-Desktop-Release/tristatekey-daemon
	tristatekey-daemon.desktop
	60-gpiod.rules"

package() {
	mkdir -p "$pkgdir"/usr/bin "$pkgdir"/etc/udev/rules.d  "$pkgdir"/home/"$USER"/.config/autostart
	cp "$srcdir"/tristatekey-daemon "$pkgdir"/usr/bin/tristatekey-daemon
	cp "$srcdir"/60-gpiod.rules "$pkgdir"/etc/udev/rules.d/60-gpiod.rules
	cp "$srcdir"/tristatekey-daemon.desktop "$pkgdir"/home/"$USER"/.config/autostart
}

sha512sums="
7ffcba055b1c510da042322324a45efff61e690315a3d0167af7bafdb42b5398a246f449e6180e166209399a33921bc8a8e26ea389fc6bdd63330d70da61b7b0  tristatekey-daemon
d0fc1a937afbe546de03d728f7b96311c9a9ef826c19baf196de0ef6e8e1b0f373cec7822118af6dfd80ea44462da312f3ded29dae0dcda9aa92fc95c0b813cf  60-gpiod.rules
4311d76c597f2c72113920926b5b487fdcad95be02316920a7c78dd214d1f04254174637bcc2b722542346c93ecf763a0523a3d03929dda11cb56b6cade11341  tristatekey-daemon.desktop
"

