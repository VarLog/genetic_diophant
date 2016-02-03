
#include <memory>

#include <gtest/gtest.h>

#include <genetic_diophant/genetic.h>

namespace genetic {

class GeneTest : public ::testing::Test {
  protected:
};

TEST_F( GeneTest, should_be_equal ) {
    const auto g1 = Gene( AlleleArray( {{1, 2, 3, 4}} ) );
    const auto g2 = Gene( AlleleArray( {{1, 2, 3, 4}} ) );

    EXPECT_TRUE( g1 == g2 );
}



class DiophantTest : public ::testing::Test {
  protected:

    using diophant_ptr_t = std::unique_ptr<Diophant>;
    diophant_ptr_t diophant_;

    static constexpr AlleleArray coefficients = AlleleArray( {{ 1, 2, 3, 4 }} );
    static constexpr Allele result = Allele( 30 );

    virtual void SetUp() {
        diophant_ = diophant_ptr_t( new Diophant( coefficients, result ) );
    }

    virtual void TearDown() {
        diophant_.reset();
    }

};

constexpr AlleleArray DiophantTest::coefficients;
constexpr Allele DiophantTest::result;


TEST_F( DiophantTest, should_solve ) {
    auto gene = Gene();

    ASSERT_NO_THROW( gene = diophant_->Solve() );

    auto actual_result = Allele( 0 );

    for( auto i = 0; i < kAllelesCount; ++i ) {
        actual_result += gene.alleles[i] * coefficients[i];
    }

    EXPECT_EQ( result, actual_result );
}


}  // namespace genetic