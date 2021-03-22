#include <Minigin.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

int main()
{
    dae::Minigin engine;
    engine.Run();
    return 0;
}
