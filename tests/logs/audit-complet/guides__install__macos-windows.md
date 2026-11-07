# guides/install/macos-windows.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | autre (contradiction) | « Trois pistes, toutes non testées » | Le tableau en liste quatre (Archives, MSYS2, vcpkg, WSL). | mineur |
| 2 | 1 | « brew install csfml » | Installation de Homebrew et du compilateur (Xcode CLT) non mentionnées ; sur macOS `gcc` est un alias de clang. | gênant |
| 3 | 2 | « Apple Silicon », « MacPorts », « MSYS2 », « vcpkg », « DLL », « `PATH` », « `.app` », « rapport » | Termes non définis ; « le rapport » = research/ du dépôt que le lecteur ne connaît pas. | mineur |
| 4 | 3 | « -I$(brew --prefix)/include -L$(brew --prefix)/lib » | Dit « probablement » : commande non vérifiée et sans contexte de ligne complète. Le rpath (`.dylib`) n'est pas traité. | mineur |
| 5 | 7 | « donne CSFML 3.0.0 avec SFML 3.0.2 », « 3.0.0.rc3-2 au 2026-11-02 » | Marqué « Non vérifié » en tête de page : conforme. | mineur |
Vérifié : liens (wsl, conventions, compiling, /guides/epitech/environment/) existent.
