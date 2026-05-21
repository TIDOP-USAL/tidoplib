#define BOOST_TEST_MODULE Tidop DataFrame test
#include <boost/test/unit_test.hpp>
#include <tidop/math/statistic/base/DataFrame.h>
#include <vector>
#include <string>

using namespace tl;

BOOST_AUTO_TEST_SUITE(DataFrameTestSuite)

BOOST_AUTO_TEST_CASE(constructor_from_rows)
{
    DataFrame<int, double, std::string> df({"ID", "Value", "Name"}, {
            {1, 1.1, "One"},
            {2, 2.2, "Two"},
            {3, 3.3, "Three"}
        });

    BOOST_CHECK_EQUAL(df.cols(), 3);
    BOOST_CHECK_EQUAL(df.rows(), 3);
    BOOST_CHECK_EQUAL(df.column(0), "ID");
    BOOST_CHECK_EQUAL(df.column(1), "Value");
    BOOST_CHECK_EQUAL(df.column(2), "Name");

    auto r0 = df.reg(0);
    BOOST_CHECK_EQUAL(std::get<0>(r0), 1);
    BOOST_CHECK_EQUAL(std::get<1>(r0), 1.1);
    BOOST_CHECK_EQUAL(std::get<2>(r0), "One");
}

BOOST_AUTO_TEST_CASE(constructor_from_vectors)
{
    std::vector<int> col1 = {1, 2};
    std::vector<double> col2 = {1.1, 2.2};
    std::vector<std::string> col3 = {"One", "Two"};

    DataFrame<int, double, std::string> df({"ID", "Value", "Name"}, col1, col2, col3);

    BOOST_CHECK_EQUAL(df.cols(), 3);
    BOOST_CHECK_EQUAL(df.rows(), 2);

    auto r1 = df.reg(1);
    BOOST_CHECK_EQUAL(std::get<0>(r1), 2);
    BOOST_CHECK_EQUAL(std::get<1>(r1), 2.2);
    BOOST_CHECK_EQUAL(std::get<2>(r1), "Two");
}

BOOST_AUTO_TEST_CASE(constructor_from_initializer_lists)
{
    DataFrame<int, double, std::string> df(
        {"ID", "Value", "Name"},
        {1, 2},
        {1.1, 2.2},
        {"One", "Two"}
    );

    BOOST_CHECK_EQUAL(df.cols(), 3);
    BOOST_CHECK_EQUAL(df.rows(), 2);
}

BOOST_AUTO_TEST_CASE(constructor_from_pairs)
{
    DataFrame<int, double, std::string> df(
        std::make_pair("ID", std::initializer_list<int>{1, 2}),
        std::make_pair("Value", std::initializer_list<double>{1.1, 2.2}),
        std::make_pair("Name", std::initializer_list<std::string>{"One", "Two"})
    );

    BOOST_CHECK_EQUAL(df.cols(), 3);
    BOOST_CHECK_EQUAL(df.rows(), 2);
    BOOST_CHECK_EQUAL(df.column(0), "ID");
    BOOST_CHECK_EQUAL(df.column(1), "Value");
    BOOST_CHECK_EQUAL(df.column(2), "Name");
}

BOOST_AUTO_TEST_CASE(size_mismatch_throws)
{
    std::vector<int> col1 = {1, 2, 3};
    std::vector<double> col2 = {1.1, 2.2};

    BOOST_CHECK_THROW((DataFrame<int, double>({"C1", "C2"}, col1, col2)), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(csv_load)
{
    Path file(Path(TL_DATA_PATH).append("sample.csv"));

    auto df = DataFrame<int, double, std::string>::fromFile(file);
    BOOST_CHECK_EQUAL(df.cols(), 3u);
    BOOST_CHECK_EQUAL(df.rows(), 2u);
    BOOST_CHECK_EQUAL(df.column(0), "col1");
    BOOST_CHECK_EQUAL(df.column(1), "col2");
    BOOST_CHECK_EQUAL(df.column(2), "col3");
    BOOST_CHECK_EQUAL(std::get<0>(df.reg(0)), 1);
    BOOST_CHECK_EQUAL(std::get<1>(df.reg(0)), 1.5);
    BOOST_CHECK_EQUAL(std::get<2>(df.reg(0)), std::string("hello"));
    BOOST_CHECK_EQUAL(std::get<0>(df.reg(1)), 2);
    BOOST_CHECK_EQUAL(std::get<1>(df.reg(1)), 2.5);
    BOOST_CHECK_EQUAL(std::get<2>(df.reg(1)), std::string("world"));


    //DataFrame<DynamicSchema> df_dyn(file_path);
}


BOOST_AUTO_TEST_SUITE_END()
