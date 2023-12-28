#include "./cossim.h"

// double dotProduct(const Vector& a, const Vector& b) {
//   return a.x * b.x + a.y * b.y;
// 	// // 16ビット整数乗算を使用することで高速化
//   // return ((long)a.x * b.x + (long)a.y * b.y + (long)a.z * b.z) / 1000.0; // オーバーフローに注意
// }

// double norm(const Vector& v) {
//   return sqrt(v.x * v.x + v.y * v.y);
// 	// // 16ビット整数乗算を使用することで高速化
// 	// return sqrt(((long)v.x * v.x + (long)v.y * v.y + (long)v.z * v.z) / 1000.0); // オーバーフローに注意
// }

// double cosineSimilarity(const Vector& a, const Vector& b) {
//   double dot = dotProduct(a, b);
//   double normA = norm(a);
//   double normB = norm(b);

//   // ゼロ除算を防ぐ
//   if (normA == 0.0 || normB == 0.0) {
//     return 0.0; // またはエラー処理を追加
//   }

//   return dot / (normA * normB);
// }
