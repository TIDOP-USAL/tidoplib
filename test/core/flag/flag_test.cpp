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
 
#define BOOST_TEST_MODULE Tidop flags test
#include <boost/test/unit_test.hpp>
#include <tidop/core/flags.h>

using namespace tl;

//enums para pruebas

// unscoped enumeration type
enum class ePrueba1 {

};

enum class ePrueba2 : int8_t
{
    flag00 = (0 << 0),
    flag01 = (1 << 0),
    flag02 = (1 << 1),
    flag03 = (1 << 2),
    flag04 = (1 << 3),
    flag05 = (1 << 4),
    flag06 = (1 << 5),
    flag07 = (1 << 6)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba2)

enum class ePrueba3 : int16_t
{
    flag00 = (0 << 0),
    flag01 = (1 << 0),
    flag02 = (1 << 1),
    flag03 = (1 << 2),
    flag04 = (1 << 3),
    flag05 = (1 << 4),
    flag06 = (1 << 5),
    flag07 = (1 << 6),
    flag08 = (1 << 7),
    flag09 = (1 << 8),
    flag10 = (1 << 9),
    flag11 = (1 << 10),
    flag12 = (1 << 11),
    flag13 = (1 << 12),
    flag14 = (1 << 13),
    flag15 = (1 << 14)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba3)


BOOST_AUTO_TEST_CASE(EnumFlags_constructors)
{
    // Constructor vacio
    EnumFlags<ePrueba2> flagEmpty;
    BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flagEmpty.flags()));

    // Constructor copia
    EnumFlags<ePrueba2> flag_copy(flagEmpty);
    BOOST_CHECK_EQUAL(0, static_cast<EnumFlags<ePrueba2>::Type>(flag_copy.flags()));

    // Constructor por enun
    EnumFlags<ePrueba2> flag(ePrueba2::flag01);

    BOOST_CHECK(flag.isDisabled(ePrueba2::flag00));
    BOOST_CHECK(flag.isEnabled(ePrueba2::flag01));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag02));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag03));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag04));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag05));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag06));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag07));
}

BOOST_AUTO_TEST_CASE(EnumFlags_assign)
{
    EnumFlags<ePrueba2> flagEmpty;
    flagEmpty = ePrueba2::flag01;

    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag01));

    flagEmpty = ePrueba2::flag02 | ePrueba2::flag03;

    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag01));
    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag02));
    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag03));

    flagEmpty = ePrueba2::flag02 | ePrueba2::flag03 | ePrueba2::flag01;
    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag00));
    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag01));
    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag02));
    BOOST_CHECK(flagEmpty.isEnabled(ePrueba2::flag03));
    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag04));
    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag05));
    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag06));
    BOOST_CHECK(flagEmpty.isDisabled(ePrueba2::flag07));
}

BOOST_AUTO_TEST_CASE(EnumFlags_clear)
{
    EnumFlags<ePrueba2> flag(ePrueba2::flag02 | ePrueba2::flag03);
    BOOST_CHECK(flag.isEnabled(ePrueba2::flag02));
    BOOST_CHECK(flag.isEnabled(ePrueba2::flag03));

    flag.clear();
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag02));
    BOOST_CHECK(flag.isDisabled(ePrueba2::flag03));
}

BOOST_AUTO_TEST_CASE(EnumFlags_flagOn)
{
    EnumFlags<ePrueba3> flag2;

    BOOST_CHECK_EQUAL(false, flag2.isEnabled(ePrueba3::flag03));

    flag2.enable(ePrueba3::flag03);

    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag03));

    //Prueba a activar un flag ya activo
    flag2.enable(ePrueba3::flag03);
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag03));
}

BOOST_AUTO_TEST_CASE(EnumFlags_flagOff)
{
    EnumFlags<ePrueba3> flag2(ePrueba3::flag15);

    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag15));

    flag2.disable(ePrueba3::flag15);

    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag15));

    //Prueba a desactivar un flag ya desactivado
    flag2.disable(ePrueba3::flag15);
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag15));
}

BOOST_AUTO_TEST_CASE(EnumFlags_switchFlag)
{
    EnumFlags<ePrueba3> flag2(ePrueba3::flag15 | ePrueba3::flag06);
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag15));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag06));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag01));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag03));

    flag2.switchFlag(ePrueba3::flag15);
    flag2.switchFlag(ePrueba3::flag06);
    flag2.switchFlag(ePrueba3::flag01);
    flag2.switchFlag(ePrueba3::flag03);

    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag15));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag06));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag01));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag03));
}

BOOST_AUTO_TEST_CASE(EnumFlags_operators)
{
    ePrueba3 flags = ePrueba3::flag15 | ePrueba3::flag06;
    EnumFlags<ePrueba3> flag2(flags);
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag15));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag06));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag01));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag03));

    flags |= ePrueba3::flag01;
    flag2 = flags;
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag15));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag06));
    BOOST_CHECK(flag2.isEnabled(ePrueba3::flag01));
    BOOST_CHECK(flag2.isDisabled(ePrueba3::flag03));

}

// Flags<T>

// Constructor Flags_8
BOOST_AUTO_TEST_CASE(Flags_8_constructor)
{
    Flags_8 flag8;
    BOOST_CHECK_EQUAL(0, flag8.flags());
    for (int i = 0; i < 8; i++)
        BOOST_CHECK(flag8.isDisabled(i));
}

// Constructor Flags_16
BOOST_AUTO_TEST_CASE(Flags_16_constructor)
{
    Flags_16 flag16;
    BOOST_CHECK_EQUAL(0, flag16.flags());
    for (int i = 0; i < 16; i++)
        BOOST_CHECK(flag16.isDisabled(i));
}

// Constructor Flags_32
BOOST_AUTO_TEST_CASE(Flags_32_constructor)
{
    Flags_32 flag32;
    BOOST_CHECK_EQUAL(0, flag32.flags());
    for (int i = 0; i < 32; i++)
        BOOST_CHECK(flag32.isDisabled(i));
}

// Constructor Flags_64
BOOST_AUTO_TEST_CASE(Flags_64_constructor)
{
    Flags_64 flag64;
    BOOST_CHECK_EQUAL(0, flag64.flags());
    for (int i = 0; i < 64; i++)
        BOOST_CHECK(flag64.isDisabled(i));
}

// Constructor copia
BOOST_AUTO_TEST_CASE(Flags_copy_constructor)
{
    Flags_16 flag16;
    flag16.enable(3);

    Flags<uint16_t> flag_copy(flag16);
    for (int i = 0; i < 16; i++) {
        if (i == 3)
            BOOST_CHECK(flag_copy.isEnabled(i));
        else
            BOOST_CHECK(flag_copy.isDisabled(i));
    }
}

// Construcción con inicialización de lista
BOOST_AUTO_TEST_CASE(Flags_initializer_list_constructor)
{
    Flags_32 flag_list{0, 3, 7, 4};
    BOOST_CHECK(flag_list.isEnabled(0));
    BOOST_CHECK(flag_list.isDisabled(1));
    BOOST_CHECK(flag_list.isDisabled(2));
    BOOST_CHECK(flag_list.isEnabled(3));
    BOOST_CHECK(flag_list.isEnabled(4));
    BOOST_CHECK(flag_list.isDisabled(5));
    BOOST_CHECK(flag_list.isDisabled(6));
    BOOST_CHECK(flag_list.isEnabled(7));
}

BOOST_AUTO_TEST_CASE(Flags_assign)
{
    Flags_8 flag1;
    flag1.enable(1);

    Flags_8 flag2 = flag1;

    BOOST_CHECK(flag2.isDisabled(0));
    BOOST_CHECK(flag2.isEnabled(1));
    BOOST_CHECK(flag2.isDisabled(2));
    BOOST_CHECK(flag2.isDisabled(3));
    BOOST_CHECK(flag2.isDisabled(4));
    BOOST_CHECK(flag2.isDisabled(5));
    BOOST_CHECK(flag2.isDisabled(6));
    BOOST_CHECK(flag2.isDisabled(7));
}

BOOST_AUTO_TEST_CASE(Flags_clear)
{
    Flags_32 flag;
    flag.enable(2);
    flag.switchFlag(3);

    BOOST_CHECK(flag.isEnabled(2));
    BOOST_CHECK(flag.isEnabled(3));

    flag.clear();

    BOOST_CHECK(flag.isDisabled(2));
    BOOST_CHECK(flag.isDisabled(3));
}

BOOST_AUTO_TEST_CASE(Flags_flagOn)
{
    Flags_64 flag2;
    BOOST_CHECK(flag2.isDisabled(15));

    flag2.enable(15);

    BOOST_CHECK(flag2.isEnabled(15));

    //Prueba a activar un flag ya activo
    flag2.enable(3);
    BOOST_CHECK(flag2.isEnabled(3));
}

BOOST_AUTO_TEST_CASE(Flags_flagOff)
{
    Flags_64 flag2{15};

    BOOST_CHECK(flag2.isEnabled(15));

    flag2.disable(15);

    BOOST_CHECK(flag2.isDisabled(15));

    //Prueba a desactivar un flag ya desactivado
    flag2.disable(15);
    BOOST_CHECK(flag2.isDisabled(15));
}

BOOST_AUTO_TEST_CASE(Flags_switchFlag)
{
    Flags_16 flag_16;
    flag_16.switchFlag(15);
    flag_16.switchFlag(6);
    BOOST_CHECK(flag_16.isEnabled(15));
    BOOST_CHECK(flag_16.isEnabled(6));
    BOOST_CHECK(flag_16.isDisabled(1));
    BOOST_CHECK(flag_16.isDisabled(3));

    flag_16.switchFlag(15);
    flag_16.switchFlag(6);
    flag_16.switchFlag(1);
    flag_16.switchFlag(3);

    BOOST_CHECK(flag_16.isDisabled(15));
    BOOST_CHECK(flag_16.isDisabled(6));
    BOOST_CHECK(flag_16.isEnabled(1));
    BOOST_CHECK(flag_16.isEnabled(3));
}

BOOST_AUTO_TEST_CASE(Flags_bad_data)
{
    Flags_16 flag_16;
    flag_16.enable(5);
    flag_16.enable(25); // Se asigna un valor fuera de rango
    BOOST_CHECK(flag_16.isDisabled(25));
    BOOST_CHECK(flag_16.isEnabled(5));
}

