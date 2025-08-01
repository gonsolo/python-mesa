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
sha256sums=('aeabc0d2487fb3f6e11dec4b05449be214d6592451467d5e545b81ba8afeb4f9')

build() {
  # This section is not needed for a pre-built wheel, so we can leave it empty or remove it.
  # If you had source code, you would compile it here.
  true
}

package() {
  python -m installer --destdir="$pkgdir" "${srcdir}/${whlname}-${pkgver}-cp313-cp313-linux_x86_64.whl"
}
