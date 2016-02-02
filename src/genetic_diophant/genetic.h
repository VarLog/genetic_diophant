#include <array>
#include <vector>

namespace genetic {
static constexpr std::size_t kAllelesCount = 4;

using Allele = unsigned int;
using AlleleArray = std::array<Allele, kAllelesCount>;

struct Gene {
    AlleleArray alleles;
    unsigned int fitness = 0;
    float likelihood = 0.f;

    Gene() {}
    Gene( AlleleArray alleles ) : alleles( std::move( alleles ) ) {}

    inline bool operator==( const Gene &that ) {
        for ( auto i = 0; i < kAllelesCount; ++i ) {
            if( this->alleles[i] != that.alleles[i] ) {
                return false;
            }
        }

        return true;
    }
};

static constexpr std::size_t kPopulationCount = 5;

using GeneContainer = std::vector<Gene>;

class Diophant {
  public:
    Diophant( AlleleArray coefficients, Allele result ) :
        coefficients_( coefficients ),
        result_( result ) {
    }

    Gene Solve() {
        return Gene();
    }

  private:
    AlleleArray coefficients_;
    Allele result_;
};

}  // namespace genetic
