/** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ SelectMultiplexer header file ]                                      #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-16                                                   #
#                                                                              #
** ************************************************************************* **/

#ifndef   __SELECTMULTIPLEXER_HPP__
#define    __SELECTMULTIPLEXER_HPP__

/*******************************************************************************
    * Includes
*******************************************************************************/
# include "IMultiplexer.hpp"
# include <sys/select.h>

/*******************************************************************************
    * Class SelectMultiplexer
*******************************************************************************/
class SelectMultiplexer : public IMultiplexer
{
public:
    SelectMultiplexer(void);
    ~SelectMultiplexer(void);

    HandleQueue		waitEvent(long long aTimeout_ms);
    void			registerHandle(const Handle& aHandle, Mode aMode);
    void			removeHandle(const Handle& aHandle, Mode aMode);

private:
    int				mMaxHandle;
    fd_set			mReadSet;
    fd_set			mWriteSet;

    HandleQueue		_collectReadyHandles(fd_set& aReadSet, fd_set& aWriteSet);
    void			_updateMaxHandleFromSets(void);
};

#endif /* __SELECTMULTIPLEXER_HPP__ */
