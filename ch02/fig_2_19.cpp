#include "stdafx.h"
#include "fig_2_19.h"

 void ch02::fig_2_19::fig_2_19_run(PrimesTreeElement::Ptr root) {
    PrimesTreeElement::Ptr tree_array[] = { root };
    tbb::parallel_for_each(tree_array,
        [](PrimesTreeElement::Ptr e,
            tbb::feeder<PrimesTreeElement::Ptr>& feeder) {
                if (e) {
                    if (e->left) feeder.add(e->left);
                    if (e->right) feeder.add(e->right);
                    if (isPrime(e->v.first))
                        e->v.second = true;
                }
        }
    );
}

bool ch02::fig_2_19::isPrime(int n) {
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

IntVector ch02::fig_2_19::makePrimesValues(int n, PrimesMap& m) {
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

PrimesTreeElement::Ptr ch02::fig_2_19::makePrimesTreeElem(int level, const IntVector& vec, IntVector::const_iterator i) {
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

PrimesTreeElement::Ptr ch02::fig_2_19::makePrimesTree(int level, IntVector& vec) {
    return makePrimesTreeElem(level, vec, vec.cbegin());
}

 bool ch02::fig_2_19::validatePrimesElem(PrimesTreeElement::Ptr e, PrimesMap& m) {
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

void ch02::fig_2_19::warmupTBB() {
    tbb::parallel_for(0, tbb::this_task_arena::max_concurrency(), [](int) {
        tbb::tick_count t0 = tbb::tick_count::now();
        while ((tbb::tick_count::now() - t0).seconds() < 0.01);
        });
}

int ch02::fig_2_19::main() {
    const int levels = 14;
    const int N = std::pow(2, levels) - 1;

    PrimesMap m;
    auto vec = makePrimesValues(N, m);
    auto root = makePrimesTree(levels, vec);

    double parallel_time = 0.0;
    {
        warmupTBB();
        tbb::tick_count t0 = tbb::tick_count::now();
        fig_2_19_run(root);
        parallel_time = (tbb::tick_count::now() - t0).seconds();
        if (!validatePrimesElem(root, m)) {
            std::cerr << "Error: incorrect results!" << std::endl;
        }
    }
    std::cout << "parallel_time == " << parallel_time << " seconds" << std::endl;
    return 0;
}
