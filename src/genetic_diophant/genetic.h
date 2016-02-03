#include <cstdlib>

#include <array>
#include <random>
#include <stdexcept>
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
        result_( result ),
        population_( kPopulationCount ) {

    }

    Gene Solve( unsigned int iteration_count_max = 50 ) {
        //std::random_device rd;
        //mt_generator_.seed( rd() );

        mt_generator_.seed( 42 );  /// \< \fixme for debug only

        for ( auto i = 0; i < kPopulationCount; ++i ) {
            population_.push_back( GenerateRandomGene() );
        }

        auto iteration_count = 0;

        while ( iteration_count++ < iteration_count_max ) {
            CalculateFitness();
            auto it = std::find_if( population_.cbegin(), population_.cend(), []( const Gene & gene ) {
                return gene.fitness == 0;
            } );

            if ( it != population_.cend() ) {
                return *it;
            }


        }

        throw new std::runtime_error( "Could not find the result gene" );
    }

  private:
    AlleleArray coefficients_;
    Allele result_;

    GeneContainer population_;

    mutable std::mt19937 mt_generator_;

    Gene GenerateRandomGene() const {
        std::uniform_int_distribution<Allele> dis( 1, result_ );
        auto gene = Gene();

        for ( auto &allele : gene.alleles ) {
            allele = dis( mt_generator_ );
        }

        return gene;
    }

    void CalculateFitness() {
        for ( auto &gene : population_ ) {
            auto res = Allele( 0 );

            for ( auto i = 0; i < kAllelesCount; ++i ) {
                res += gene.alleles[i] * coefficients_[i];
            }

            auto fitness = std::abs( static_cast<int>( result_ - res ) );

            gene.fitness = fitness;
        }
    }
};

}  // namespace genetic
