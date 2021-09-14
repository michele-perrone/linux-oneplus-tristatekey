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
		tristatekey-daemon.desktop"

package() {
	mkdir -p "$pkgdir"/usr/bin "$pkgdir"/etc/udev/rules.d  "$pkgdir"/home/"$USER"/.config/autostart
	cp "$srcdir"/tristatekey-daemon "$pkgdir"/usr/bin/tristatekey-daemon
	cp "$srcdir"/tristatekey-daemon.desktop "$pkgdir"/home/"$USER"/.config/autostart
}

sha512sums="
7ffcba055b1c510da042322324a45efff61e690315a3d0167af7bafdb42b5398a246f449e6180e166209399a33921bc8a8e26ea389fc6bdd63330d70da61b7b0  tristatekey-daemon
4311d76c597f2c72113920926b5b487fdcad95be02316920a7c78dd214d1f04254174637bcc2b722542346c93ecf763a0523a3d03929dda11cb56b6cade11341  tristatekey-daemon.desktop
"

