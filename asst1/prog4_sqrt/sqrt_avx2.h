#include <immintrin.h>

#define VECTOR_SIZE 32

__m256 _mm256_abs(__m256 x) {
    static const __m256 sign_bit = _mm256_set1_ps(-0.0f);
    return _mm256_andnot_ps(sign_bit, x);
}

void sqrt_avx2(int N,
                float initialGuess,
                float values[],
                float output[]) {
    static const __m256 kThreshold = _mm256_set1_ps(0.00001f);
    static const __m256 one = _mm256_set1_ps(1.0f);
    static const __m256 half = _mm256_set1_ps(0.5f);
    static const __m256 three = _mm256_set1_ps(3.0f);
    __m256 x, guess, pred, mask;
    for (int i = 0; i < N; i += VECTOR_SIZE) {
        x = _mm256_load_ps(values + i);
        guess = _mm256_set1_ps(initialGuess);
        pred = _mm256_abs(_mm256_sub_ps(_mm256_mul_ps(_mm256_mul_ps(guess, guess), x), one));
        mask = _mm256_cmp_ps(kThreshold, pred, 0x1);
        while(_mm256_movemask_ps(mask)) {
            guess = _mm256_blendv_ps(guess,
                _mm256_mul_ps(_mm256_sub_ps(_mm256_mul_ps(three, guess),
                    _mm256_mul_ps(_mm256_mul_ps(_mm256_mul_ps(x, guess), guess), guess)), half),
                mask);
            pred = _mm256_abs(_mm256_sub_ps(_mm256_mul_ps(_mm256_mul_ps(guess, guess), x), one));
            mask = _mm256_cmp_ps(kThreshold, pred, 0x1);
        }
        _mm256_store_ps(output + i, _mm256_mul_ps(x, guess));
    }
}