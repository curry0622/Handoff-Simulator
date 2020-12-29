#include<bits/stdc++.h>
using namespace std;

int basePosX[4] = {330, 640, 360, 660};
int basePosY[4] = {350, 310, 680, 658};
int entranceX[36] = {
    100, 200, 300, 400, 500, 600, 700, 800, 900,
    0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000, 0, 1000,
    100, 200, 300, 400, 500, 600, 700, 800, 900
};
int entranceY[36] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    100, 100, 200, 200, 300, 300, 400, 400, 500, 500, 600, 600, 700, 700, 800, 800, 900, 900,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000
};

class car {
public:
    // variables
    bool out;
    int posX;
    int posY;
    double power[5];
    int base[5];
    string dir;
    double maxPower;
    int maxBase;


    // functions
    // constructor
    car(int x, int y) {
        out = false;
        posX = x;
        posY = y;
        initDir();
        initBaseAndPower();
    }

    // initial direction
    void initDir() {
        if(posX == 0)
            dir = "right";
        else if(posX == 1000)
            dir = "left";
        else if(posY == 0)
            dir = "down";
        else if(posY == 1000)
            dir = "up";
    }

    // calculate distance to base i
    double calDistToBaseI(int i) {
        return sqrt(pow(posX - basePosX[i], 2) + pow(posY - basePosY[i], 2));
    }

    // calculate power from base i
    double calPowerFromBaseI(int i) {
        return 100 - 20 * log10(calDistToBaseI(i)) - 32.45;
    }

    // calculate max power and base of current position
    void calMax() {
        maxPower = 0;
        maxBase = 0;
        for(int i = 0; i < 4; i++) {
            if(maxPower < calPowerFromBaseI(i)) {
                maxPower = calPowerFromBaseI(i);
                maxBase = i;
            }
        }
    }

    // initial base
    void initBaseAndPower() {
        int initBase = 0;
        double minDist = 1000;
        double initPower = 0;
        for(int i = 0; i < 4; i++) {
            double distToBaseI = calDistToBaseI(i);
            if(minDist > distToBaseI) {
                minDist = distToBaseI;
                initBase = i;
            }
        }
        initPower = calPowerFromBaseI(initBase);
        for(int i = 0; i < 5; i++) {
            base[i] = initBase;
            power[i] = initPower;
        }
    }

    // switch direction
    void switchDir(string turnDir) {
        int dirIndex = 0;
        if(dir == "up")
            dirIndex = 0;
        else if(dir == "down")
            dirIndex = 1;
        else if(dir == "left")
            dirIndex = 2;
        else
            dirIndex = 3;

        int turnIndex = 0;
        if(turnDir == "straight")
            turnIndex = 0;
        else if(turnDir == "left")
            turnIndex = 1;
        else
            turnIndex = 2;

        switch(dirIndex) {
            case 0:
                switch(turnIndex) {
                    case 0:
                        dir = "up";
                        break;
                    case 1:
                        dir = "left";
                        break;
                    case 2:
                        dir = "right";
                        break;
                }
                break;
            case 1:
                switch(turnIndex) {
                    case 0:
                        dir = "down";
                        break;
                    case 1:
                        dir = "right";
                        break;
                    case 2:
                        dir = "left";
                        break;
                }
                break;
            case 2:
                switch(turnIndex) {
                    case 0:
                        dir = "left";
                        break;
                    case 1:
                        dir = "down";
                        break;
                    case 2:
                        dir = "up";
                        break;
                }
                break;
            case 3:
                switch(turnIndex) {
                    case 0:
                        dir = "right";
                        break;
                    case 1:
                        dir = "up";
                        break;
                    case 2:
                        dir = "down";
                        break;
                }
                break;
        }
    }
};

int calSumOfIntVec(const vector<int>& vec) {
    int sum = 0;
    for(const auto& v: vec)
        sum += v;
    return sum;
}

int main() {
    // inputs
    double lambda = 0;
    cout << "lambda = ";
    cin >> lambda;
    double threshold = 0;
    cout << "threshold = ";
    cin >> threshold;
    double entropy = 0;
    cout << "entropy = ";
    cin >> entropy;
    double threshold2 = 0;
    cout << "threshold2 = ";
    cin >> threshold2;
    double entropy2 = 0;
    cout << "entropy2 = ";
    cin >> entropy2;
    int totalTime = 0;
    cout << "total time = ";
    cin >> totalTime;

    // start clock
    int startClk = clock();

    // poisson distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> d(lambda);

    // initial setup
    srand(time(NULL));
    int carNum = 0;
    vector<car> carVec;
    double totalPower[5] = {0, 0, 0, 0, 0};
    vector<int> handoffEachSecOfPolicy[5];

    // for each second
    for(int t = 0; t < totalTime; t++) {

        // for each entry
        for(int i = 0; i < 36; i++) {
            int newCarNum = d(gen);
            for(int j = 0; j < newCarNum; j++)
                carVec.push_back(car(entranceX[i], entranceY[i]));
        }

        // push back handoff each second
        for(int i = 0; i < 5; i++)
            handoffEachSecOfPolicy[i].push_back(0);

        // for each car
        for(auto& c: carVec) {
            // if this car isn't out of range
            if(!c.out) {
                // if both posX and posY % 100 == 0, switch direction
                if(c.posX % 100 == 0 && c.posY % 100 == 0) {
                    double p = double(rand()) / RAND_MAX;
                    if(p < 0.6)
                        c.switchDir("straight");
                    else if(p < 0.8)
                        c.switchDir("left");
                    else
                        c.switchDir("right");
                }

                // new position of this car
                if(c.dir == "up")
                    c.posY -= 10;
                else if(c.dir == "down")
                    c.posY += 10;
                else if(c.dir == "left")
                    c.posX -= 10;
                else
                    c.posX += 10;

                // if this is car is out of range
                if(c.posX < 0 || c.posX > 1000 || c.posY < 0 || c.posY > 1000) {
                    c.out = true;
                    continue;
                }

                // policy 0 -> when Pold < Pmin, handoff
                double oldPower = c.calPowerFromBaseI(c.base[0]);
                if(oldPower >= 10) {
                    // no handoff
                    c.power[0] = oldPower;
                } else {
                    c.calMax();
                    if(c.base[0] != c.maxBase) {
                        // handoff
                        handoffEachSecOfPolicy[0].back() += 1;
                    }
                    c.power[0] = c.maxPower;
                    c.base[0] = c.maxBase;
                }

                // policy 1 -> when Pold < Pnew, handoff
                c.calMax();
                if(c.base[1] == c.maxBase) {
                    // no handoff
                    c.power[1] = c.maxPower;
                } else {
                    // handoff
                    handoffEachSecOfPolicy[1].back() += 1;
                    c.power[1] = c.maxPower;
                    c.base[1] = c.maxBase;
                }

                // policy 2 -> when Pold < threshold and Pold < Pnew, handoff
                oldPower = c.calPowerFromBaseI(c.base[2]);
                if(oldPower >= threshold) {
                    // no handoff
                    c.power[2] = oldPower;
                } else {
                    c.calMax();
                    if(c.base[2] != c.maxBase) {
                        // handoff
                        handoffEachSecOfPolicy[2].back() += 1;
                    }
                    c.power[2] = c.maxPower;
                    c.base[2] = c.maxBase;
                }

                // policy 3 -> when Pnew > Pold + entropy, handoff
                oldPower = c.calPowerFromBaseI(c.base[3]);
                c.calMax();
                if(c.maxPower > oldPower + entropy) {
                    // handoff
                    handoffEachSecOfPolicy[3].back() += 1;
                    c.power[3] = c.maxPower;
                    c.base[3] = c.maxBase;
                } else {
                    // no handoff
                    c.power[3] = oldPower;
                }

                // policy 4 -> when Pold < threshold2 or Pnew > Pold + entropy2, handoff
                oldPower = c.calPowerFromBaseI(c.base[4]);
                c.calMax();
                if(oldPower >= threshold2 && c.maxPower <= oldPower + entropy2) {
                    // no handoff
                    c.power[4] = oldPower;
                } else {
                    // handoff
                    handoffEachSecOfPolicy[4].back() += 1;
                    c.power[4] = c.maxPower;
                    c.base[4] = c.maxBase;
                }

                // add total power
                for(int i = 0; i < 5; i++)
                    totalPower[i] += c.power[i];

                // add one car
                carNum += 1;
            }
        }
    }

    cout << "|\ntotal car number = " << carNum << endl;
    for(int i = 0; i < 5; i++) {
        cout << fixed;
        cout << "|\n";
        cout << "handoff of policy[" << i << "] = " << calSumOfIntVec(handoffEachSecOfPolicy[i]) << endl;
        cout << "total power of policy[" << i << "] = " << setprecision(2) << totalPower[i] << endl;
        cout << "average power of policy[" << i << "] = " << setprecision(2) << totalPower[i] / carNum << endl;
    }
    cout << "|\ncondition = " << setprecision(2) << (totalPower[0] / carNum + totalPower[1] / carNum) / 2 << endl;
    int endClk = clock();
    cout << "|\nexecution time is " << (endClk - startClk) / double(CLOCKS_PER_SEC) << endl;
    /*
    ofstream fout;
    for(int i = 1; i <= 4; i++) {
        fout.open(to_string(lambda) + "_q" + to_string(i) + ".csv");
        for(const auto& h: handoffEachSecOfPolicy[i]) {
            fout << h << '\n';
        }
        fout.close();
    }
    */
    return 0;
}

