# Contributor: Michele Perrone <perrone.michele@outlook.com>
# Maintainer: Michele Perrone <perrone.michele@outlook.com>
pkgname=tristatekey-daemon
pkgver=0.2.0
pkgrel=0
pkgdesc="A daemon that hooks up the tri-state key on the OnePlus6(T)"
url="https://github.com/michele-perrone/postmarketos-oneplus6-tristatekey"
arch="all"
license="GPL"
install="$pkgname.post-install"
builddir="$srcdir"/build
source="tristatekey-daemon/CMakeLists.txt
	tristatekey-daemon/tristatekey-root-daemon.c
	tristatekey-daemon/tristatekey-user-daemon.c
	tristatekey-root-daemon.initd
	tristatekey-user-daemon.desktop"

build() {
	cmake -S "$srcdir" -B "$builddir"
	make -C "$builddir"
}

package() {
	mkdir -p "$pkgdir"/usr/bin "$pkgdir"/etc/xdg/autostart
	cp "$builddir"/tristatekey-root-daemon "$pkgdir"/usr/bin/tristatekey-root-daemon
	cp "$builddir"/tristatekey-user-daemon "$pkgdir"/usr/bin/tristatekey-user-daemon
	cp "$srcdir"/tristatekey-user-daemon.desktop "$pkgdir"/etc/xdg/autostart
	install -m755 -D "$srcdir"/tristatekey-root-daemon.initd "$pkgdir"/etc/init.d/tristatekey-root-daemon
}

sha512sums="
2f6dbdf3cee19059fab6379f736e504d98a803be8bb0c15b9f279184c89fee2da4536107c9385e2e0d704e6872a8aae77cf61276fa4b01775d808df20daba204  CMakeLists.txt
315a5158449c15b26e7c4239018ea63519f5e2bc5ba1e33c935fe9fafbe4bfa3e5e3c5c700d245e8b48a7b3b0534de08d270bb1149ec5204d360211f68c630a7  tristatekey-root-daemon.c
8e3e97b79f1931873d1dd9b3f2dee360839e905ebf887a7f2b221a22fde0139d3e9984b3c84e2c35d09ab69e041dd050472a2be1f9d60c5978f795b6d6aba7a9  tristatekey-user-daemon.c
069a7c916db16f038430e3e19bc9d3eaa89b680fe8eff123ebfa50a00e1dcde4ba81bf298f43ae76cd64e0025e9911add7d098f518bf27fec936efd59ef719af  tristatekey-root-daemon.initd
1f323fc53f51119dabf3c81c4657685c767a1614068b1747607fb518a573f9d62d4ff26feb4e707157376de65512d8051fbddd67082e3e3abfe835e084693f87  tristatekey-user-daemon.desktop
"
