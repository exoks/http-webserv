/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ IProtocolFactory Interface ]                                        #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-07-09                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __IPROTOCOLFACTORY_HPP__
#define    __IPROTOCOLFACTORY_HPP__

/*******************************************************************************
    * Includes :
*******************************************************************************/
# include "shared_ptr.hpp"
# include "typedefs.hpp"

# include "ICluster.hpp"
# include "Directive.hpp"

/*******************************************************************************
    * IProtocolFactory Interface :
*******************************************************************************/

class IProtocolFactory
{
public:
    typedef mem::shared_ptr<IProtocolFactory>		SharedPtr;

    virtual ~IProtocolFactory(void) {};
    virtual ICluster*   createCluster(Directive::SharedPtr aGlobalDir) = 0;
};

#endif /* __IPROTOCOLFACTORY_HPP__ */