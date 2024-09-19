#include "stdafx.h"
#include "fig_2_18.h"

 void ch02::fig_2_18::serialCheckPrimesElem(PrimesTreeElement::Ptr e) {
    if (e) {
        if (isPrime(e->v.first))
            e->v.second = true;
        if (e->left) serialCheckPrimesElem(e->left);
        if (e->right) serialCheckPrimesElem(e->right);
    }
}

  bool ch02::fig_2_18::isPrime(int n) {
     int e = std::sqrt(n);
     std::vector<bool> p(e + 1, true);

     for (int i = 2; i <= e; ++i) {
         if (p[i]) {
             if (n % i) {
                 for (int j = 2 * i; j <= e; j += i) {
                     p[j] = false;
                 }
             }
             else {
                 return false;
             }
         }
     }
     return true;
 }

   IntVector ch02::fig_2_18::makePrimesValues(int n, PrimesMap& m) {
      std::default_random_engine gen;
      std::uniform_int_distribution<int> dist;
      IntVector vec;

      for (int i = 0; i < n; ++i) {
          int v = dist(gen);
          vec.push_back(v);
          m[v] = isPrime(v);
      }
      return vec;
  }

    PrimesTreeElement::Ptr ch02::fig_2_18::makePrimesTreeElem(int level, const IntVector& vec, IntVector::const_iterator i) {
       if (level && i != vec.cend()) {
           PrimesTreeElement::Ptr e = std::make_shared<PrimesTreeElement>(PrimesValue(*i, false));
           if (level - 1) {
               e->left = makePrimesTreeElem(level - 1, vec, ++i);
               e->right = makePrimesTreeElem(level - 1, vec, ++i);
           }
           return e;
       }
       else {
           return nullptr;
       }
   }

     PrimesTreeElement::Ptr ch02::fig_2_18::makePrimesTree(int level, IntVector& vec) {
        return makePrimesTreeElem(level, vec, vec.cbegin());
    }

      bool ch02::fig_2_18::validatePrimesElem(PrimesTreeElement::Ptr e, PrimesMap& m) {
         if (e) {
             if (m[e->v.first] != e->v.second) {
                 return false;
             }
             if (!validatePrimesElem(e->left, m) || !validatePrimesElem(e->right, m)) {
                 return false;
             }
         }
         return true;
     }

       int ch02::fig_2_18::main() {
          const int levels = 14;
          const int N = std::pow(2, levels) - 1;
          PrimesMap m;
          auto vec = makePrimesValues(N, m);
          auto root = makePrimesTree(levels, vec);

          double serial_time = 0.0;
          {
              tbb::tick_count t0 = tbb::tick_count::now();
              serialCheckPrimesElem(root);
              serial_time = (tbb::tick_count::now() - t0).seconds();
              if (!validatePrimesElem(root, m)) {
                  std::cerr << "Error: incorrect results!" << std::endl;
              }
          }
          std::cout << "serial_time == " << serial_time << " seconds" << std::endl;
          return 0;
      }
