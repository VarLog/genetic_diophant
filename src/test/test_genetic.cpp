
#include <memory>

#include <gtest/gtest.h>

#include <genetic_diophant/genetic.h>

namespace genetic {

class GeneTest : public ::testing::Test {
  protected:
};

TEST_F( GeneTest, should_be_equal ) {
    auto g1 = Gene( allele_array_t( {{1, 2, 3, 4}} ) );
    auto g2 = Gene( allele_array_t( {{1, 2, 3, 4}} ) );

    EXPECT_TRUE( g1 == g2 );
}



class DiophantTest : public ::testing::Test {
  protected:

    using diophant_ptr_t = std::unique_ptr<Diophant>;
    diophant_ptr_t diophant_;

    static constexpr allele_array_t coefficients = allele_array_t( {{ 1, 2, 3, 4 }} );
    static constexpr allele_t result = allele_t( 30 );

    virtual void SetUp() {
        diophant_ = diophant_ptr_t( new Diophant( coefficients, result ) );
    }

    virtual void TearDown() {
        diophant_.reset();
    }

};

constexpr allele_array_t DiophantTest::coefficients;
constexpr allele_t DiophantTest::result;


TEST_F( DiophantTest, should_solve ) {
    auto gene = diophant_->Solve();

    auto actual_result = allele_t( 0 );

    for( auto i = 0; i < alleles_count; ++i ) {
        actual_result += gene.alleles[0] * coefficients[0];
    }

    EXPECT_EQ( result, actual_result );
}


}  // namespace genetic