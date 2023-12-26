#include "./cossim.h"

// void setup() {
//   Serial.begin(9600);
  
//   // 例として2つのベクトルを用意
//   Vector vectorA = {1, 2, 3};
//   Vector vectorB = {4, 5, 6};

//   // コサイン類似度を計算
//   double similarity = cosineSimilarity(vectorA, vectorB);

//   // 結果をシリアルモニタに表示
//   Serial.print("Cosine Similarity: ");
//   Serial.println(similarity, 4); // 4は小数点以下の桁数
// }

double dotProduct(const Vector& a, const Vector& b) {
  return a.x * b.x + a.y * b.y;
	// // 16ビット整数乗算を使用することで高速化
  // return ((long)a.x * b.x + (long)a.y * b.y + (long)a.z * b.z) / 1000.0; // オーバーフローに注意
}

double norm(const Vector& v) {
  return sqrt(v.x * v.x + v.y * v.y);
	// // 16ビット整数乗算を使用することで高速化
	// return sqrt(((long)v.x * v.x + (long)v.y * v.y + (long)v.z * v.z) / 1000.0); // オーバーフローに注意
}

double cosineSimilarity(const Vector& a, const Vector& b) {
  double dot = dotProduct(a, b);
  double normA = norm(a);
  double normB = norm(b);

  // ゼロ除算を防ぐ
  if (normA == 0.0 || normB == 0.0) {
    return 0.0; // またはエラー処理を追加
  }

  return dot / (normA * normB);
}
