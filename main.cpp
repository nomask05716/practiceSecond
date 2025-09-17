#include <iostream>
#include "crypto.h"
#include "math.h"
#include "validInput.h"
using namespace std;
enum mainMenu {
    decompositionDegree = 1,
    euclidAlgo,  
    euclidAlgoForC,
    cryptoProtocol,
    chainSht,
    MitM
};
int main(){
    bool running =true;
    int choiceMenu;
    while (running) {  
        choiceMenu = ValidInputMainMenu();
        switch (choiceMenu) {
            case decompositionDegree: {
                int footing = exp(),degr = degree(),mod = modul();
                bool ferma = threoremFerma(footing,degr,mod);
                if (ferma == 1){
                    cout << "a^x mod p = 1"<<endl;
                }else{
                    int res = powMod(footing,degr,mod);
                    cout << "a^x mod p =" << res <<endl;
                }
                break;
            }
            case euclidAlgo: {
                int u,v;
                int a = exp(), b = exp();
                int gcd_val = extendedGcd(a,b,u,v);
                cout << "u = " << u << " v = " << v<<endl;
                break;
            }
            case euclidAlgoForC:{
                int c = exp(),mod = modul();
                int d = searchReciprocalNum(c,mod);
                cout << "d = "<<d<<endl;
                break;
            }
            case cryptoProtocol:{
                shamirProtocol();
                break;
            }
            case chainSht:{
                int a =exp(),b = exp();
                vector<int> res= chainShotes(a,b);
                cout << " [";
                size_t sizeVec = res.size();
                for (int i : res ){
                    if (sizeVec>1){
                        cout << i << ", ";
                        sizeVec -=1;
                    }else {
                        cout <<i;
                    }
                    
                }
                cout << "]"<<endl;
                diophantineEquation();
                break;
            }
            case MitM:{
                mitmAttack();
                break;
            }
            
        }
        char continueChoice;
        cout << "\nХотите продолжить? (y/n): ";
        cin >> continueChoice;
        cin.ignore(); 
        if (continueChoice != 'y' && continueChoice != 'Y') {
            running = false;
            cout << "Выход из программы..." << endl;
        }
        
    }
}