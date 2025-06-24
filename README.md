# GameInC

**GameInC**, öğrenim amaçlı geliştirilmiş bir OpenGL tabanlı Minecraft klonudur.  
Tüm özellikleri tam olarak desteklemese de, aşağıdaki temel işlevleri içerir:

- Rastgele dünya oluşturma 
- Multithreaded chunk yükleme  
- Blok kırma  
- Blok yerleştirme 

Bu özellikler basit düzeyde çalışmaktadır ve öğrenim amacıyla geliştirilmiştir.

---

## Kurulum Talimatları

### Gereksinimler

- C/C++ derleyicisi (örneğin: GCC, Clang veya Visual Studio C++)
- Eğer Clang derleyicisini **Windows'ta** kullanacaksanız, [Ninja Build Tools](https://ninja-build.org/) gereklidir.
- CMake
- Python betiklerini çalıştırmak için [`uv`](https://github.com/astral-sh/uv) Python paket yöneticisi

### Depoyu Klonlayın

```bash
git clone https://github.com/karabulutcann/GameInC.git
cd GameInC
```

### Derleme (Python `uv` ile)

```bash
uv sync
uv run build.py
```

> Build script'i tamamlandıktan sonra oyun otomatik olarak başlatılır.

### Derleme (CMake ile manuel olarak)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
