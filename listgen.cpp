extern "C" {
__attribute__((import_name("log")))
extern void log(int);
}

constexpr const int SIZE = 1024;
int buffer[SIZE];

void memcpy(int* dst, const char* src, unsigned int count) {
  for (unsigned int offset = 0; offset < count; ++offset) {
    dst[offset] = src[offset];
  }
}

__attribute__((export_name("get_offset")))
int* get_offset() {
  return buffer;
}

__attribute__((export_name("listgen")))
void listgen() {
    memcpy(buffer, "<div>hi</div>", 13);
}
