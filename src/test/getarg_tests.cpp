#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testGingerCoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-gng");
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", false));
    BOOST_CHECK(GetBoolArg("-gng", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-gngo"));
    BOOST_CHECK(!GetBoolArg("-gngo", false));
    BOOST_CHECK(GetBoolArg("-gngo", true));

    ResetArgs("-gng=0");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", false));
    BOOST_CHECK(!GetBoolArg("-gng", true));

    ResetArgs("-gng=1");
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", false));
    BOOST_CHECK(GetBoolArg("-gng", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nogng");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", false));
    BOOST_CHECK(!GetBoolArg("-gng", true));

    ResetArgs("-nogng=1");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", false));
    BOOST_CHECK(!GetBoolArg("-gng", true));

    ResetArgs("-gng -nogng");  // -gng should win
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", false));
    BOOST_CHECK(GetBoolArg("-gng", true));

    ResetArgs("-gng=1 -nogng=1");  // -gng should win
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", false));
    BOOST_CHECK(GetBoolArg("-gng", true));

    ResetArgs("-gng=0 -nogng=0");  // -gng should win
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", false));
    BOOST_CHECK(!GetBoolArg("-gng", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--gng=1");
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", false));
    BOOST_CHECK(GetBoolArg("-gng", true));

    ResetArgs("--nogng=1");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", false));
    BOOST_CHECK(!GetBoolArg("-gng", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-gng", "eleven"), "eleven");

    ResetArgs("-gng -bar");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-gng", "eleven"), "");

    ResetArgs("-gng=");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-gng", "eleven"), "");

    ResetArgs("-gng=11");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-gng", "eleven"), "11");

    ResetArgs("-gng=eleven");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-gng", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-gng", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-gng", 0), 0);

    ResetArgs("-gng -bar");
    BOOST_CHECK_EQUAL(GetArg("-gng", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-gng=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-gng", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-gng=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-gng", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--gng");
    BOOST_CHECK_EQUAL(GetBoolArg("-gng"), true);

    ResetArgs("--gng=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-gng", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nogng");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", true));
    BOOST_CHECK(!GetBoolArg("-gng", false));

    ResetArgs("-nogng=1");
    BOOST_CHECK(!GetBoolArg("-gng"));
    BOOST_CHECK(!GetBoolArg("-gng", true));
    BOOST_CHECK(!GetBoolArg("-gng", false));

    ResetArgs("-nogng=0");
    BOOST_CHECK(GetBoolArg("-gng"));
    BOOST_CHECK(GetBoolArg("-gng", true));
    BOOST_CHECK(GetBoolArg("-gng", false));

    ResetArgs("-gng --nogng");
    BOOST_CHECK(GetBoolArg("-gng"));

    ResetArgs("-nogng -gng"); // gng always wins:
    BOOST_CHECK(GetBoolArg("-gng"));
}

BOOST_AUTO_TEST_SUITE_END()
