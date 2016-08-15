/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Tools library.
**
** Licensees holding valid commercial KD Tools licenses may use this file in
** accordance with the KD Tools Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the GNU
** Lesser General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.LGPL included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.net if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#define KDGENERICFACTORYBASE_INSTANTIATION
#include "kdgenericfactory.h"

#include <QMap>

// explicit instantiation of common types:
template class KDGenericFactoryBase<QString,QHash>;
template class KDGenericFactoryBase<QString,QMap>;
template class KDGenericFactoryBase<int,QHash>;
template class KDGenericFactoryBase<int,QMap>;

/*!
   \class KDGenericFactory
   \ingroup core
   \brief Template based generic factory implementation
   \since_c 2.2

   (The exception safety of this class has not been evaluated yet.)

   KDGenericFactory is an implemention of of the factory pattern. It can be used to
   "produce" instances of different classes having a common superclass 
   T_Product. The user of the
   factory registers those producable classes in the factory by using an identifier
   (T_Identifier, defaulting to QString). That identifer can then be used to
   produce as many instances of the registered product as he wants.

   The advanced user can even choose the type of the map the factory is using to store its
   FactoryFunctions by passing a T_Map template parameter. It defaults to QHash. KDGenericFactory
   expects it to be a template class accepting T_Identifier and FactoryFunction as parameters.
   Additionally it needs to provide:

   \li\link QHash::const_iterator a nested %const_iterator \endlink typedef for an iterator type that when dereferenced has type ((const) reference to) FactoryFunction (Qt convention),
   \li\link QHash::insert %insert( T_Identifier, FactoryFunction ) \endlink, which must overwrite any existing entries with the same identifier.
   \li\link QHash::find %find( T_Identifier ) \endlink, 
   \li\link QHash::end %end() \endlink,
   \li\link QHash::size %size() \endlink, 
   \li\link QHash::remove %remove( T_Identifier ) \endlink, and 
   \li\link QHash::keys %keys ) \endlink, returning a QList<T_Identifier>.

   The only two class templates that currently match this concept are
   QHash and QMap. QMultiHash and QMulitMap do not work, since they
   violate the requirement on insert() above, and std::map and
   std::unordered_map do not match because they don't have keys() and
   because a dereferenced iterator has type
   std::pair<const T_Identifier,FactoryFunction>
   instead of just FactoryFunction.

   \section general-use General Use
    
   The following example shows how the general use case of KDGenericFactory looks like:

   \code

   class Fruit
   {
   };

   class Apple : public Fruit
   {
   };

   class Pear : public Fruit
   {
   };

   int main()
   {
       // creates a common fruit "factory"
       KDGenericFactory< Fruit > fruitPlantation;
       // registers the product "Apple"
       fruitPlantation.registerProduct< Apple >( "Apple" );
       // registers the product "Pear"
       fruitPlantation.registerProduct< Pear >( "Pear" );

       // lets create some stuff - here comes our tasty apple:
       Fruit* myApple = fruitPlantation.create( "Apple" );
       
       // and a pear, please:
       Fruit* myPear = fruitPlantation.create( "Pear" );
       
       // ohh - that doesn't work, returns a null pointer:
       Fruit* myCherry = fruitPlantation.create( "Cherry" );
   }

   \endcode
*/

/*!
   \fn KDGenericFactory::~KDGenericFactory

   Destructor.
*/

/*!
    \typedef KDGenericFactory::FactoryFunction
    
    This typedef defines a factory function producing an object of type T_Product.
*/

/*!
   \fn KDGenericFactory::registerProduct( const T_Identifier& name )

   Registers a product of type T, identified by \a name in the factory.
   Any type with the same name gets unregistered.

   If a product was registered via this method, it will be created using its 
   default constructor.
*/

/*!
   \fn KDGenericFactory::unregisterProduct( const T_Identifier& name )

   Unregisters the previously registered product identified by \a name from the factory.
   If no such product is known, nothing is done.
*/

/*!
   \fn KDGenericFactory::productCount() const

   Returns the number of different products known to the factory.
*/

/*!
   \fn KDGenericFactory::availableProducts() const

   Returns the list of products known to the factory.
*/

/*!
   \fn KDGenericFactory::create( const T_Identifier& name ) const

   Creates and returns a product of the type identified by \a name.
   Ownership of the product is transferred to the caller.
*/

/*!
   \fn KDGenericFactory::registerProductionFunction( const T_Identifier& name, FactoryFunction create )

   Subclasses can use this method to register their own FactoryFunction \a create to create products of
   type T, identified by \a name. When a product is registered via this method, it will be created
   by calling create().
*/

#ifdef KDTOOLSCORE_UNITTESTS

#include <KDUnitTest/test.h>

#include <QStringList>
#include <QMap>
#include <QObject>

class Fruit
{
public:
    virtual ~Fruit() {}
};

class Apple : public Fruit
{
};

class Pear : public Fruit
{
};

class Orange : public QObject, public Fruit // multiple inheritance
{
};

static inline std::ostream& operator<<( std::ostream& stream, const QStringList& list )
{
    stream << "QStringList(";
    for( QStringList::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        stream << " " << it->toLocal8Bit().data();
        if( it + 1 != list.end() )
            stream << ",";
    }
    stream << " )";
    return stream;
}

class KDGenericFactoryTest : public KDUnitTest::Test {
public:
    KDGenericFactoryTest() : Test( "KDGenericFactory" ) {}
    void run() {
        doRun<QHash>();
        doRun<QMap>();
    }

    template <template <typename U, typename V> class T_Map>
    void doRun();
};

KDAB_EXPORT_UNITTEST( KDGenericFactoryTest, "kdtools/core" )

template <template <typename U, typename V> class T_Map>
void KDGenericFactoryTest::doRun() {

    {
        KDGenericFactory< Fruit, QString, T_Map > fruitPlantation;
        assertEqual( fruitPlantation.productCount(), 0U );
        assertEqual( fruitPlantation.availableProducts(), QStringList() );

        fruitPlantation.template registerProduct< Apple >( QLatin1String( "Apple" ) );
        assertEqual( fruitPlantation.productCount(), 1U );
        assertEqual( fruitPlantation.availableProducts(), QStringList( QLatin1String( "Apple" ) ) );

        fruitPlantation.template registerProduct< Pear >( QLatin1String( "Pear" ) );
        assertEqual( fruitPlantation.productCount(), 2U );

        fruitPlantation.template registerProduct< Orange >( QLatin1String( "Orange" ) );
        assertEqual( fruitPlantation.productCount(), 3U );

        Fruit* fruit = 0;
        fruit = fruitPlantation.create( QLatin1String( "Apple" ) );
        assertNotNull( fruit );
        assertNotNull( dynamic_cast< Apple* >( fruit ) );

        fruit = fruitPlantation.create( QLatin1String( "Pear" ) );
        assertNotNull( fruit );
        assertNotNull( dynamic_cast< Pear* >( fruit ) );

        fruit = fruitPlantation.create( QLatin1String( "Orange" ) );
        assertNotNull( fruit );
        assertNotNull( dynamic_cast< Orange* >( fruit ) );

        fruit = fruitPlantation.create( QLatin1String( "Cherry" ) );
        assertNull( fruit );

        fruitPlantation.unregisterProduct( QLatin1String( "Apple" ) );
        assertEqual( fruitPlantation.productCount(), 2U );
        fruit = fruitPlantation.create( QLatin1String( "Apple" ) );
        assertNull( fruit );

        fruit = fruitPlantation.create( QLatin1String( "Pear" ) );
        assertNotNull( fruit );
        assertNotNull( dynamic_cast< Pear* >( fruit ) );


        fruitPlantation.unregisterProduct( QLatin1String( "Pear" ) );
        assertEqual( fruitPlantation.productCount(), 1U );
        fruit = fruitPlantation.create( QLatin1String( "Pear" ) );
        assertNull( fruit );
    }

    {
        // Check that using registerProductionFunction() works (=compiles)
        class Factory : public KDGenericFactory< Fruit, QString > {
            static Fruit * badfood() { return reinterpret_cast<Fruit*>( 0xBadF00d ); }
        public:
            void registerBadPear() {
                this->registerProductionFunction( QLatin1String("Pear"), badfood );
            }
        };

        static Fruit * badfruit = reinterpret_cast<Fruit*>( 0xBadF00d );
;
        Factory f;
        f.template registerProduct<Apple>( QLatin1String("Apple") );
        f.registerBadPear();
        //assertNotEqual( f.create( QLatin1String("Apple") ), badfruit );
        assertEqual( f.create( QLatin1String("Pear") ), badfruit );
    }

}
#endif // KDTOOLSCORE_UNITTESTS
