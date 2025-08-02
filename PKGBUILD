# Maintainer: Your Name <your.email@example.com>

pkgname=python-mesa3d
whlname=python_mesa3d
pkgver=0.1.0
pkgrel=1
pkgdesc="A Python binding for Mesa"
arch=('x86_64')
url="https://github.com/gonsolo/python-mesa3d"
license=('GPL')
depends=('python')
makedepends=('python-build' 'python-installer')
source=("${whlname}-${pkgver}-cp313-cp313-linux_x86_64.whl")
sha256sums=('b3217117db25d7c089e5b68cfb97f522d661f49e390105e7c14c7abb521cc97a')

build() {
  # This section is not needed for a pre-built wheel, so we can leave it empty or remove it.
  # If you had source code, you would compile it here.
  true
}

package() {
  python -m installer --destdir="$pkgdir" "${srcdir}/${whlname}-${pkgver}-cp313-cp313-linux_x86_64.whl"
}
