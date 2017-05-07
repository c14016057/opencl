typedef unsigned int uint32_t;
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
		return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
		return (rotate_left(m, key&31) + key)^key;
}

__kernel void mydot(int k1, int k2, __global uint32_t *C, int N, int NUMT) {
		int idx = get_global_id(0), len = N/NUMT + 1, l = idx*len,
			r = (l+len) < N ? (l+len) : N;
		uint32_t count = 0;
		for(int i = l; i < r; i++)
			count += encrypt(i, k1) * encrypt(i, k2);
		C[idx] = count;
}

