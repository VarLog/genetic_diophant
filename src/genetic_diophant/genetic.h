#include <cstdlib>

#include <array>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

namespace genetic {
static constexpr std::size_t kAllelesCount = 4;

using Allele = unsigned int;
using AlleleArray = std::array<Allele, kAllelesCount>;

struct Gene {
    AlleleArray alleles;
    unsigned int fitness = 0;
    double likelihood = 0.;

    Gene() {}
    Gene( AlleleArray alleles ) : alleles( std::move( alleles ) ) {}

    inline bool operator==( const Gene &that ) const {
        for ( auto i = 0; i < kAllelesCount; ++i ) {
            if( this->alleles[i] != that.alleles[i] ) {
                return false;
            }
        }

        return true;
    }
};

static constexpr std::size_t kPopulationCount = 25;

using GeneContainer = std::vector<Gene>;

class Diophant {
  public:
    Diophant( AlleleArray coefficients, Allele result ) :
        coefficients_( coefficients ),
        result_( result ) {
        population_.reserve( kPopulationCount );
    }

    Gene Solve( unsigned int iteration_count_max = 500 ) {
        std::random_device rd;
        mt_generator_.seed( rd() );

        //mt_generator_.seed( 42 );  /// \< for debuging only

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

            CalculateLikelihoods();

            CreateNewPopulation();
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

    void CalculateLikelihoods() {
        auto sum = 0.;

        for ( const auto &gene : population_ ) {
            sum += 1. / static_cast<double>( gene.fitness );
        }

        for ( auto &gene : population_ ) {
            auto likelihood = 1. / static_cast<double>( gene.fitness ) / sum;
            gene.likelihood = likelihood;
        }
    }

    void CreateNewPopulation() {
        auto population_new = GeneContainer( );
        population_new.reserve( kPopulationCount );

        for( auto i = 0; i < kPopulationCount; ++i ) {
            auto parents = ChoiseParents();
            auto child = Crossover( parents );
            population_new.push_back( child );
        }

        std::move( population_new.begin(), population_new.end(), population_.begin() );
    }

    using Parents = std::pair<Gene, Gene>;

    Parents ChoiseParents() const {

        auto parent1 = Gene();

        {
            std::uniform_real_distribution<double> dis( 0., 1. );

            auto prob = dis( mt_generator_ );
            auto likelihood_sum = 0.;

            for ( const auto &gene : population_ ) {
                if ( prob >= likelihood_sum && prob <= likelihood_sum + gene.likelihood ) {
                    parent1 = gene;
                    break;
                }

                likelihood_sum += gene.likelihood;
            }
        }

        auto parent2 = Gene();

        {
            std::uniform_real_distribution<double> dis( 0., 1. - parent1.likelihood );

            auto prob = dis( mt_generator_ );
            auto likelihood_sum = 0.;

            for ( const auto &gene : population_ ) {
                if ( gene == parent1 ) {
                    continue;
                }

                if ( prob >= likelihood_sum && prob <= likelihood_sum + gene.likelihood ) {
                    parent2 = gene;
                    break;
                }

                likelihood_sum += gene.likelihood;
            }
        }

        return Parents( parent1, parent2 );
    }

    static constexpr float kMutationProbability = .05;

    Gene Crossover( const Parents &parents ) const {
        std::uniform_int_distribution<std::size_t> dis_separator( 1, kAllelesCount - 1 );

        auto separator_index = dis_separator( mt_generator_ );

        std::uniform_int_distribution<int> dis_order( 0, 1 );

        auto order = dis_order( mt_generator_ );

        auto child = Gene();
        auto parent = Gene();

        if ( order ) {
            child = std::move( parents.first );
            parent = std::move( parents.second );
        }
        else {
            child = std::move( parents.second );
            parent = std::move( parents.first );
        }

        for ( auto i = separator_index; i < kAllelesCount; ++i ) {
            child.alleles[i] = parent.alleles[i];
        }

        Mutation( child );

        return child;
    }

    void Mutation( Gene &gene ) const {
        std::uniform_real_distribution<double> dis_mutation( 0., 1. );
        auto prob_mutation = dis_mutation( mt_generator_ );

        if ( prob_mutation <= kMutationProbability ) {
            std::uniform_int_distribution<std::size_t> dis_mutation_allele_index( 0, kAllelesCount - 1 );
            auto allele_index = dis_mutation_allele_index( mt_generator_ );

            std::uniform_int_distribution<Allele> dis_mutation_allele( 1, result_ );
            auto allele = dis_mutation_allele( mt_generator_ );

            gene.alleles[allele_index] = allele;
        }
    }
};

}  // namespace genetic
