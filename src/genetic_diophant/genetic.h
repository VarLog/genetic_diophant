#include <array>
#include <vector>

namespace genetic {
static constexpr std::size_t alleles_count = 4;

using allele_t = unsigned int;
using allele_array_t = std::array<allele_t, alleles_count>;

struct Gene {
    allele_array_t alleles;
    unsigned int fitness = 0;
    float likelihood = 0.f;

    Gene() {}
    Gene( allele_array_t alleles ) : alleles( std::move( alleles ) ) {}

    inline bool operator==( const Gene &that ) {
        for ( auto i = 0; i < alleles_count; ++i ) {
            if( this->alleles[i] != that.alleles[i] ) {
                return false;
            }
        }

        return true;
    }
};

static constexpr std::size_t population_count = 5;

using gene_container_t = std::vector<Gene>;

class Diophant {
  public:
    Diophant( allele_array_t coefficients, allele_t result ) :
        coefficients_( coefficients ),
        result_( result ) {
    }

    Gene Solve() {
        return Gene();
    }

  private:
    allele_array_t coefficients_;
    allele_t result_;
};

}  // namespace genetic
