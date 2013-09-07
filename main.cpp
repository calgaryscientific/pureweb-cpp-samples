//
// Copyright (c) 2011 Calgary Scientific Inc., all rights reserved.
//

#include "DDx.h"
        
int main(int argc, char* argv[]) 
{
    try
    {
        CSI::AutoLibraryInitializer csiLibInit;
        DDx app;
        
        app.Go();
    }
    catch (CSI::Exception const & e)
    {
        std::cerr << e.what() << std::endl;
        return 13;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 13;
    }
    catch (...)
    {
        std::cerr << "Unhandled unknown exception ... exiting." << std::endl;
        return 13;
    }
    return 0;
}
