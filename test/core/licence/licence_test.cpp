/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop licence test
#include <boost/test/unit_test.hpp>
#include <tidop/core/app/licence.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(LicenceTestSuite)

struct LicenceTest
{

    LicenceTest()
        : license2(nullptr)
    {
    }

    ~LicenceTest()
    {
        if (license2) {
            delete license2;
            license2 = nullptr;
        }
    }

    void setup()
    {
        _license_text = "                   GNU LESSER GENERAL PUBLIC LICENSE"
                        "                       Version 3, 29 June 2007"
                        ""
                        "Copyright(C) 2007 Free Software Foundation, Inc. < https://fsf.org/>"
                        "Everyone is permitted to copy and distribute verbatim copies"
                        "of this license document, but changing it is not allowed.";
                       
        license2 = new Licence("PhotoMatch", "LGPL-3.0 license");
        license2->setUrl("https://github.com/TIDOP-USAL/photomatch");
        license2->setAuthor("author");
        license2->setAuthorEmail("user@email.com");
        license2->setVersion("1.0.0");
        license2->setLicenseText(_license_text);
    }

    void teardown()
    {

    }

    Licence license;
    Licence *license2;
    std::string _license_text;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, LicenceTest)
{
    BOOST_CHECK_EQUAL(true, license.empty());
}

BOOST_FIXTURE_TEST_CASE(constructor, LicenceTest)
{
    BOOST_CHECK_EQUAL(false, license2->empty());
}

BOOST_FIXTURE_TEST_CASE(product_name, LicenceTest)
{
    BOOST_CHECK_EQUAL("PhotoMatch", license2->productName());
}

BOOST_FIXTURE_TEST_CASE(license_name, LicenceTest)
{
    BOOST_CHECK_EQUAL("LGPL-3.0 license", license2->licenseName());
}

BOOST_FIXTURE_TEST_CASE(license_text, LicenceTest)
{
    BOOST_CHECK_EQUAL(_license_text, license2->licenseText());
}

BOOST_FIXTURE_TEST_CASE(version, LicenceTest)
{
    BOOST_CHECK_EQUAL("1.0.0", license2->version());
}

BOOST_FIXTURE_TEST_CASE(get_author, LicenceTest)
{
    BOOST_CHECK_EQUAL("author", license2->autor());
}

BOOST_FIXTURE_TEST_CASE(get_url, LicenceTest)
{
    BOOST_CHECK_EQUAL("https://github.com/TIDOP-USAL/photomatch", license2->url());
}

BOOST_FIXTURE_TEST_CASE(get_author_email, LicenceTest)
{
    BOOST_CHECK_EQUAL("user@email.com", license2->authorEmail());
}

BOOST_FIXTURE_TEST_CASE(set_text, LicenceTest)
{
    license2->setLicenseText("LGPL-3.0 license");
    BOOST_CHECK_EQUAL("LGPL-3.0 license", license2->licenseText());
}

BOOST_FIXTURE_TEST_CASE(set_version, LicenceTest)
{
    license2->setVersion("2.0.0");
    BOOST_CHECK_EQUAL("2.0.0", license2->version());
}

BOOST_FIXTURE_TEST_CASE(set_author, LicenceTest)
{
    license2->setAuthor("author2");
    BOOST_CHECK_EQUAL("author2", license2->autor());
}

BOOST_FIXTURE_TEST_CASE(set_url, LicenceTest)
{
    license2->setUrl("https://github.com/TIDOP-USAL/tidoplib");
    BOOST_CHECK_EQUAL("https://github.com/TIDOP-USAL/tidoplib", license2->url());
}

BOOST_FIXTURE_TEST_CASE(set_author_email, LicenceTest)
{
    license2->setAuthorEmail("user2@email.com");
    BOOST_CHECK_EQUAL("user2@email.com", license2->authorEmail());
}


BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(AppLicenceTestSuite)

struct AppLicenceTest
{

    AppLicenceTest()
        : appLicense(nullptr)
    {
    }

    ~AppLicenceTest()
    {
        if (appLicense) {
            delete appLicense;
            appLicense = nullptr;
        }
    }

    void setup()
    {

        _license_text = "                   GNU LESSER GENERAL PUBLIC LICENSE"
                        "                       Version 3, 29 June 2007"
                        ""
                        "Copyright(C) 2007 Free Software Foundation, Inc. < https://fsf.org/>"
                        "Everyone is permitted to copy and distribute verbatim copies"
                        "of this license document, but changing it is not allowed.";
        appLicense = new AppLicence;
        appLicense->setProductName("GRAPHOS");
        appLicense->setLicence("GPL-3.0 license");
        appLicense->setUrl("https://github.com/TIDOP-USAL/graphos");
        appLicense->setAuthor("author");
        appLicense->setAuthorEmail("user@email.com");
        appLicense->setVersion("2.0.0");
        appLicense->setLicenseText(_license_text);

        appLicense->push_back(Licence("Qt5", "GNU Lesser General Public License v3.0"));
        appLicense->push_back(Licence("OpenCV", "Apache-2.0 license"));
        appLicense->push_back(Licence("Boost", "Boost Software License 1.0"));
        appLicense->push_back(Licence("TidopLib", "GNU Lesser General Public License v3.0"));

    }

    void teardown()
    {

    }

    AppLicence *appLicense;
    std::string _license_text;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, AppLicenceTest)
{
    BOOST_CHECK_EQUAL(false, appLicense->empty());
}

BOOST_FIXTURE_TEST_CASE(get_product_name, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("GRAPHOS", appLicense->productName());
}

BOOST_FIXTURE_TEST_CASE(get_license_name, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("GPL-3.0 license", appLicense->licenseName());
}

BOOST_FIXTURE_TEST_CASE(get_license_text, AppLicenceTest)
{
    BOOST_CHECK_EQUAL(_license_text, appLicense->licenseText());
}

BOOST_FIXTURE_TEST_CASE(get_version, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("2.0.0", appLicense->version());
}

BOOST_FIXTURE_TEST_CASE(get_author, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("author", appLicense->autor());
}

BOOST_FIXTURE_TEST_CASE(get_url, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("https://github.com/TIDOP-USAL/graphos", appLicense->url());
}

BOOST_FIXTURE_TEST_CASE(get_author_email, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("user@email.com", appLicense->authorEmail());
}

BOOST_FIXTURE_TEST_CASE(set_text, AppLicenceTest)
{
    appLicense->setLicenseText("GPL-3.0 license");
    BOOST_CHECK_EQUAL("GPL-3.0 license", appLicense->licenseText());
}

BOOST_FIXTURE_TEST_CASE(set_version, AppLicenceTest)
{
    appLicense->setVersion("3.0.0");
    BOOST_CHECK_EQUAL("3.0.0", appLicense->version());
}

BOOST_FIXTURE_TEST_CASE(set_author, AppLicenceTest)
{
    appLicense->setAuthor("author2");
    BOOST_CHECK_EQUAL("author2", appLicense->autor());
}

BOOST_FIXTURE_TEST_CASE(set_url, AppLicenceTest)
{
    appLicense->setUrl("https://github.com/TIDOP-USAL/graphos3");
    BOOST_CHECK_EQUAL("https://github.com/TIDOP-USAL/graphos3", appLicense->url());
}

BOOST_FIXTURE_TEST_CASE(set_author_email, AppLicenceTest)
{
    appLicense->setAuthorEmail("user2@email.com");
    BOOST_CHECK_EQUAL("user2@email.com", appLicense->authorEmail());
}

BOOST_FIXTURE_TEST_CASE(licenses_size, AppLicenceTest)
{
    BOOST_CHECK_EQUAL(4, appLicense->size());
}

BOOST_FIXTURE_TEST_CASE(clear, AppLicenceTest)
{
    appLicense->clear();
    BOOST_CHECK_EQUAL(0, appLicense->size());
}

BOOST_FIXTURE_TEST_CASE(begin, AppLicenceTest)
{
    BOOST_CHECK_EQUAL("Qt5", appLicense->begin()->productName());
}

BOOST_FIXTURE_TEST_CASE(push_back, AppLicenceTest)
{
    appLicense->push_back(Licence("COLMAP", "BSD-3"));
    BOOST_CHECK_EQUAL(5, appLicense->size());
}

BOOST_FIXTURE_TEST_CASE(iterate, AppLicenceTest)
{
    auto license = appLicense->begin();
    BOOST_CHECK_EQUAL("Qt5", license->productName());
    license++;
    BOOST_CHECK_EQUAL("OpenCV", license->productName());
    license++;
    BOOST_CHECK_EQUAL("Boost", license->productName());
    license++;
    BOOST_CHECK_EQUAL("TidopLib", license->productName());
    license++;
    BOOST_CHECK(license == appLicense->end());
}

BOOST_AUTO_TEST_SUITE_END()


