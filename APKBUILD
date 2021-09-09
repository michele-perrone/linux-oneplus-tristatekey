# Contributor: Michele Perrone <perrone.michele@outlook.com>
# Maintainer: Michele Perrone <perrone.michele@outlook.com>
pkgname=tristatekey-daemon
pkgver=0.1.0
pkgrel=0
pkgdesc="A daemon that hooks up the tri-state key on the OnePlus6(T)"
url="https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey"
arch="aarch64"
license="GPL"

source="build-tristatekey-daemon-Desktop-Release/tristatekey-daemon
	tristatekey-daemon.initd
	tristatekey-daemon.confd"

package() {
	mkdir -p "$pkgdir"/etc/init.d "$pkgdir"/usr/bin
	install -m755 -D "$srcdir"/tristatekey-daemon.initd "$pkgdir"/etc/init.d/tristatekey-daemon
	install -m755 -D "$srcdir"/tristatekey-daemon.confd "$pkgdir"/etc/conf.d/tristatekey-daemon
	cp "$srcdir"/tristatekey-daemon "$pkgdir"/usr/bin/tristatekey-daemon
}

sha512sums="
7ffcba055b1c510da042322324a45efff61e690315a3d0167af7bafdb42b5398a246f449e6180e166209399a33921bc8a8e26ea389fc6bdd63330d70da61b7b0  tristatekey-daemon
32f242c3d8e803068c554c18d3bc324bd507f0b20153dcaf5105da70bc7830622427e7d43a6a5b0a5d9be7b606e93248479d6c24a80e7b14e16ec440179b9e9c  tristatekey-daemon.initd
70f1e1f9df6f0250c4ce71c373a4d20bbe357c8a16377156fac31aec632f3ad1816216413a1a92bb1e493a658df0f156022c265717d3f66a2c8c00b85afa8f4f  tristatekey-daemon.confd
"
