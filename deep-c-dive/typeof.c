#define SAFE_SWAP(a, b) do {         \
    typeof(a) temp = a;              \
    a = b;                           \
    b = temp;                        \
} while (0)

int main() {
		int a, b;
		SAFE_SWAP(a, b);
		float c,d;
		SAFE_SWAP(c, d);
		return 0;
}
