#include<iostream>
#include<string>
#include<vector>
#include<ctime>　//時刻や経過時間の取得に使用
#include<cstdlib> //rand関数の使用に使った．
#include <opencv2/opencv.hpp> //画像表示のための外部ライブラリ（一般公開されているもの）
using namespace cv;
using namespace std;

//Itemを管理するクラス
class Item {
protected:
    string name;
public:
    Item(string n) : name(n) {}
    string getName()const { return name; }
};

//武器のクラス（攻撃力を管理）
class Weapon : public Item {
private:
    int attackPower;
public:
    Weapon(string n, int ap) : Item(n), attackPower(ap) {}//コンストラクタを定義
    int getattackPower()const { return attackPower; }
};

// 防具クラス（被ダメージを管理）
class Armor : public Item {
private:
    int defensePower;  // 防御力
public:
    Armor(string n, int dp) : Item(n), defensePower(dp) {}
    int getDefensePower() const { return defensePower; }
};

//回復アイテムのクラス（回復量を管理）
class HealingItem : public Item {
private:
    int healingPower;
public:
    HealingItem(string n, int h) : Item(n), healingPower(h) {} //コンストラクタを定義
    int gethealingPower() const { return healingPower; }
};

//PlayerとEnemyの共通クラス
class Common {
protected:
    string name;
    int HP;
public:
    Common(string n, int hp) : name(n), HP(hp) {}
    //ダメージ計算の関数　
    virtual void  takedamage(int damage) { HP -= damage; };
    //生存を確認する関数（値を返して処理）
    virtual bool stillAlive() const { return HP > 0; };
    virtual int getBaseAttackPower() const { return 0; }//基礎攻撃力を取得する関数
    string getName()const { return name; }//名前取得関数

};

//Player クラス
class Hero : public Common {
private:
    int money;
    Weapon weapon;
    Armor armor;
    vector<Item> Inventory;  //Itemを管理するベクター
    vector<HealingItem> healingItems; //healingItemを管理するベクター
    time_t startTime;//ゲ－ムの開始時刻をクラス構築時に取得
public:
    Hero(string n, int hp, int m, Weapon w, Armor a) : Common(n, hp), money(m), weapon(w), armor(a)
    {
        startTime = time(nullptr);   //ゲームの開始時間から測る
    }

    int getMoney() const { return money; }

    void earnMoney(int amount) {
        money += amount;
    }
    void spendMoney(int amount) {
        money -= amount;
    }

    //ステータス確認関数
    void showStatus() {
        cout << "名前:" << name << "\nHP:" << HP << "\n所持金" << money << "\n武器:" << weapon.getName() << "\n攻撃力"
            << weapon.getattackPower() << "\n";
        cout << "防具:" << armor.getName() << "\n防御力:" << armor.getDefensePower() << "\n";
        displayInventory();
        displayhealingItems();
    };

    //アイテムをInventoryベクターに追加
    void addItem(const Item& item) {
        Inventory.push_back(item);
    }
    //回復アイテムをhealingItemsベクターに追加
    void addhealingitem(const HealingItem& healingitem) {
        healingItems.push_back(healingitem);
    }
    //インベントリをイテレーターを用いて表示する関数
    void displayInventory() {
        cout << "獲得アイテム（武器，防具）:";
        for (auto it = Inventory.begin();it != Inventory.end(); it++) {
            cout << it->getName() << " ";
        }
        cout << endl;
    }
    //回復アイテムをイテレーターを用いて表示する関数
    void displayhealingItems() {
        cout << "獲得した回復アイテム:";
        for (auto it = healingItems.begin();it != healingItems.end(); it++) {
            cout << it->getName() << " ";
        }
        cout << endl;
    }
    //Commonクラスからオーバーライド
    void takedamage(int damage) override {
        HP -= damage;
    }

    bool stillAlive() const override { return HP > 0; }//生存確認関数

    //HPを回復する関数
    void heal(int amount) {
        HP += amount;
        if (HP > 350) {//HPの上限を300に設定
            HP = 350;
        }
    }

    //敵との戦闘を行う関数
    void battle(Common& enemy) {
        srand(time(0));//乱数の値を初期化

        while (stillAlive() && enemy.stillAlive()) {//自分か敵が死ぬまで戦闘継続
            int baseDamage = rand() % 10 + 1;//基本ダメージを10～１の間でランダムに決定
            int playerDamage = baseDamage + weapon.getattackPower();//武器の攻撃力を加算
            int enemyBaseDamage = rand() % 10 + 1;//敵のダメージに乱数で10～１の間のブレを付ける
            int enemyDamage = enemyBaseDamage + enemy.getBaseAttackPower();//敵の基礎攻撃力を加算（モンスターごとに決定）
            //プレイヤーの攻撃
            cout << "あなたの攻撃！" << weapon.getName() << "で敵を攻撃する！　ダメージ: " << playerDamage << "\n";
            enemy.takedamage(playerDamage);//敵がダメージを受ける
            if (!enemy.stillAlive()) {
                cout << enemy.getName() << "を倒した！\n";
                break;
            }
            //防具の防御力分ダメージを軽減
            int reducedDamage = enemyDamage - armor.getDefensePower();
            if (reducedDamage < 0) {
                reducedDamage = 0;
            }
            //敵の攻撃
            cout << "敵の攻撃！ダメージ: " << reducedDamage << "\n";
            takedamage(reducedDamage); //プレイヤーがダメージを受ける
            cout << "残りのHP" << HP << "\n";
            if (!stillAlive()) {
                cout << "ゲームオーバー\n";
            }
        }
    }

    //プレイ時間を表示するための関数
    void showPlayTime() const {
        //現在時刻の取得
        time_t currentTime = time(nullptr);
        //プレイ時間の計算
        double playTime = difftime(currentTime, startTime);
        cout << "遊んだ時間" << playTime << "秒\n";
    }
    //武器を装備するための関数(強い方を装備）
    void equipWeapon(const Weapon& newWeapon) {
        if (newWeapon.getattackPower() >= weapon.getattackPower()) {
            weapon = newWeapon;
            cout << "新しい武器:" << weapon.getName() << "を装備した！\n";
            //新しい武器をインベントリに追加
            addItem(newWeapon);
        }
        else {
            cout << "残念!今もっている武器の方が，強かった!\n";
        }
    }
    //防具を装備するための関数（強い方を装備）
    void equipArmor(Armor& newArmor) {
        if (newArmor.getDefensePower() >= armor.getDefensePower()) {
            armor = newArmor;
            cout << "新しい防具:" << armor.getName() << "を装備した！\n";
            addItem(newArmor);
        }
        else {
            cout << "残念!今もっている防具の方が，強かった!\n";
        }
    }
    //回復アイテムを使用するための関数
    void usehealingItem() {
        if (!healingItems.empty()) {//回復アイテムを所持しているかの確認 
            HealingItem healingItem = healingItems.back();
            healingItems.pop_back();
            heal(healingItem.gethealingPower());   //heal関数でHPを回復
            cout << healingItem.getName() << "を使用して，HPを" << healingItem.gethealingPower() << "回復しました\n";
            cout << "現在のHP:" << HP << "\n";

        }
        else {
            cout << "回復アイテムを持っていません！\n";
        }
    }
    //インベントリベクターを取得
    const vector<Item>& getInventory() const {
        return Inventory;
    }

    //選択子が終わり，ゲームが終わったときに表示
    void Storyend() {
        cout << "冒険が,,,終わった,,,\n";
        showStatus();
        showPlayTime();
        cout << "遊んでくれてありがとう!別ルートもあるから遊んでみてね！\n";
    }

};


//Enemyクラス：敵の情報管理
class Enemy : public Common {
private:
    int baseattackPower; //敵の基礎攻撃力を指定
public:
    Enemy(string n, int hp, int ap) : Common(n, hp), baseattackPower(ap) {}
    //ダメージを受ける関数(override)
    void takedamage(int damage) override { HP -= damage; }
    //生存確認関数
    bool stillAlive() const override { return HP > 0; }
    int getBaseAttackPower() const override { return baseattackPower; }
};

// メニュー表示関数
void displayMenu() {
    cout << "1. ステータス確認\n";
    cout << "2. 冒険に戻る\n";
    cout << "3. 回復アイテムを使う\n";
    cout << "4. ゲームを終了する\n";
}

// メニュー操作関数
void handleMenu(Hero& player) {
    int menuChoice;//メニューの選択肢を管理する変数
    bool inMenu = true; //trueの間開き続ける
    while (inMenu) {
        displayMenu();//選択肢を選ばせる
        cout << "選択肢を入力してください: ";
        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            player.showStatus();
            break;
        case 2:
            inMenu = false;//メニューを閉じる
            break;
        case 3:
            player.usehealingItem();
            break;
        case 4:
            player.Storyend();
            exit(0);
        default:
            cout << "無効な選択肢です。再度選択してください。\n";
        }
    }
}


//モンスターの画像表示のための関数
void displaySliym() {
    Mat img = imread("C:/Users/81702/image/183818.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaybat() {
    Mat img = imread("C:/Users/81702/image/185028.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaySeaCucumer() {
    Mat img = imread("C:/Users/81702/image/190719.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayghost() {
    Mat img = imread("C:/Users/81702/image/191254.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaysubordinate() {
    Mat img = imread("C:/Users/81702/image/192953.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaymaoueasy() {
    Mat img = imread("C:/Users/81702/image/maou.png");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayDoragon() {
    Mat img = imread("C:/Users/81702/image/200926.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayDoragon() {
    Mat img = imread("C:/Users/81702/image/200926.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayarchDemon() {
    Mat img = imread("C:/Users/81702/image/202012.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

//選択式ストーリーを展開するための関数
void advanceNormalStory(Hero& player, int depth = 0) {// depthによってストーリーの深さを管理(Storyendのdepthは分岐の行の数＋1)
    if (depth == 6) {
        player.Storyend();
        return;

    }
    int choice;//選択肢を管理する変数
    vector<vector<string>> storyChoices = {//二次元ベクターでストーリーの選択肢を管理
        {"お店の手伝いをする","辺りを散策する","近くに人がいるので話す"},
        {"冒険に出る","宝箱を探す","通りを曲がって武器屋を探す"},
        {"洞窟に入る","森の中に入る","海の方に行く"},
        {"食料を探す","古い館に入る","墓地を散策する" },
        {"魔王と戦う","ドラゴンと戦う","デーモンと戦う"},
        {"故郷に戻ってのんびり暮らす","俺より強いやつを探しに行く","商人になって稼ぐ"}


    };
    cout << "選択肢:\n";
    for (int i = 0; i < storyChoices[depth].size(); ++i) {
        cout << i + 1 << ". " << storyChoices[depth][i] << "\n";//二次元ベクターのdepth行i列成分を順に表示し，ストーリー分岐を提示
    }
    cout << "4.メニューを開く\n";

    cout << "キーボードで選択肢を選んでね:";
    cin >> choice;
    //Switch文の外に置くことで，毎選択肢(depthの進行度）ごとにメニュー表示の選択ができるようにした
    if (choice == 4) {
        handleMenu(player);
        advanceNormalStory(player, depth);//メニュー選択後に元のストーリーの進行度合い(depth)に応じた選択肢の表示に戻れる
        return;
    }

    //ストーリーの進行
    switch (depth) {
    case 0:
        if (choice == 1) {
            "バ先の女将:冒険だのなんだの言ってないでさっさと定職見つけなさいな\n";
            player.earnMoney(100);
            cout << "100G手に入れた!\n";
        }
        else if (choice == 2) {
            cout << "あたりを散策すると，何かを見つけた\n";
            HealingItem potion("傷薬", 20);
            player.addhealingitem(potion);
            cout << "傷薬を見つけた\n";
        }
        else if (choice == 3) {
            cout << "お姉さんに話しかけた!\n"
                << "お姉さん:武器を持っていないようだけど,冒険は初めて？このあたりのモンスターは弱いけど，武器をもって挑んだ方がいいわよ\n"
                << "そこの通りを曲がったところに武器屋があるから，買って行くといいわよ!\n";
        }
        break;

    case 1:
        if (choice == 1) {
            displaySliym();
            cout << "スライムがいた！戦おう！\n";
            Enemy enemy("スライム", 10, 3);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
        }
        else if (choice == 2) {
            cout << "小さな宝箱を開けた！中には" << "木の棒が入っていた\n";
            Weapon newWeapon("木の棒", 5);
            player.equipWeapon(newWeapon);
        }
        else if (choice == 3) {
            cout << "武器屋を見つけた\n" << "商人:いまなら武器と防具セットで,100Gだよう!\n";
            cout << "所持金は" << player.getMoney() << "です.\n";
            cout << "1.購入する\n" << "2.止めておく\n";
            int selectbuy;
            cin >> selectbuy;
            if (selectbuy == 1) {
                if (player.getMoney() >= 100) {
                    player.spendMoney(100);
                    cout << "100G払った．\n";
                    Weapon newWeapon("鉄の剣", 15);
                    Armor newArmor("鉄の鎧", 5);
                    player.equipWeapon(newWeapon);
                    player.equipArmor(newArmor);
                }
                else if (player.getMoney() < 100) {
                    cout << "お金が足りなかった…\n";
                }
            }
            else if (selectbuy == 2) {
                cout << "買うのは，やめておこう…（貯金しよ）\n";
            }
        }
        break;
    case 2:
        if (choice == 1) {
            displaybat();
            cout << "洞窟の中には，危険なコウモリがたくさんいる…\n"
                << "デンジャラスコウモリが，襲い掛かってきた\n";
            Enemy enemy("デンジャラスコウモリ", 30, 5);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
        }
        else if (choice == 2) {
            HealingItem ElfMedicine("エルフの回復薬", 80);
            player.addhealingitem(ElfMedicine);
            cout << "なんと，森の中にはエルフがいた\n" << "エルフの回復薬を貰った，これはすごく効果がありそうだ\n";
        }
        else if (choice == 3) {
            displaySeaCucumer();
            cout << "海の浅瀬に，デンジャラスナマコが出現した\n";
            Enemy enemy("デンジャラスナマコ", 40, 4);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "デンジャラスナマコを倒したら，何か落としたぞ!\n";
            Armor newArmor("ナマコアーマー", 8);
            player.equipArmor(newArmor);

        }
        break;
    case 3:
        if (choice == 1) {
            cout << "木の上を探して食料を見つけた\n";
            HealingItem mysteriousfruit("謎の木の実", 15);
            player.addhealingitem(mysteriousfruit);
            cout << "謎の木の実を見つけた\n";
        }
        else if (choice == 2) {
            displayghost();
            cout << "館の中は，真っ暗だ\n";
            cout << "もっていた松明で，明かりをつけた\n";
            cout << "下っ端お化けbが出現した\n";
            Enemy enemy("下っ端お化けb", 50, 7);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
        }
        else if (choice == 3) {
            cout << "墓荒らしをしていると,何かを見つけた\n";
            cout << "呪いのリングを見つけた\n";
            Weapon newWeapon("呪いのリング", 25);
            player.equipWeapon(newWeapon);
        }
        break;
    case 4:
        if (choice == 1) {
            cout << "魔王の城へ，突入した\n";
            displaysubordinate();
            cout << "魔王の下っ端が現れた\n";
            Enemy enemy("魔王の下っ端", 50, 6);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "魔王の下っ端を倒した\n" << "魔王の待つ玉座に,一直線だ\n";
            displaymaoueasy();
            cout << "魔王(Normal)が現れた，Normalなので弱そうだ\n";
            cout << "よく来たな" << player.getName() << "よ，ここでくたばるがいい!!\n";
            Enemy enemy2("魔王(Normal)", 100, 10);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "魔王を倒しちゃった，こんなものか,,,\n";
        }
        else if (choice == 2) {
            cout << "竜の住む山に向かった\n"
                << "小さいドラゴンがいる,捕獲しよう\n";
            displayDoragon();
            cout << "背後に黒い影が!!母ドラゴンが飛んできた\n" << "母は強し，戦うしかない!\n";
            Enemy enemy("母ドラゴン", 95, 12);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "母ドラゴンを倒した，てこずらせやがって，このガキは高く売れそうだぜ\n";
        }
        else if (choice == 3) {
            cout << "太古からある，悪魔の遺跡にやってきた\n"
                << "不気味な文様の横に,何やら，危なそうな赤いスイッチがある，押しますか\n"
                << "1.押す\n" << "2.押す\n";
            int switchselect;
            cin >> switchselect;
            if (switchselect == 1 || switchselect == 2) {
                displayarchDemon();
                cout << "アークデーモンが現れた，なんでスイッチ押しちゃったんだろう\n";
                Enemy enemy("アークデーモン", 93, 10);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//プレイヤーが死んだときはここで終了
                }
            }

        }
        break;

    case 5:
        if (choice == 1) {
            cout << player.getName() << "は，慎ましくしっぽり暮らした\n";
        }
        else if (choice == 2) {
            cout << "さらに強い相手とやりたいなら，Hardモードがおすすめだよ!\n";
        }
        else if (choice == 3) {
            cout << player.getName() << "には，商才があった！なんと商人の方が向いていたのだ,にくい男め！\n";
            cout << "1000000G,稼いだ\n";
            player.earnMoney(1000000);
        }
        break;

    }
    advanceNormalStory(player, depth + 1); //次のステージへ進む
}



void displayRadioFish() {
    Mat img = imread("C:/Users/81702/image/224829.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayOak() {
    Mat img = imread("C:/Users/81702/image/231311.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayskeleton() {
    Mat img = imread("C:/Users/81702/image/232133.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaymimic() {
    Mat img = imread("C:/Users/81702/image/234222.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayRiche() {
    Mat img = imread("C:/Users/81702/image/002325.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayOrochi() {
    Mat img = imread("C:/Users/81702/image/010216.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayhomunculus() {
    Mat img = imread("C:/Users/81702/image/013133.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displaygolem() {
    Mat img = imread("C:/Users/81702/image/015508.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayCerberus() {
    Mat img = imread("C:/Users/81702/image/022809.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}
void displaymaou() {
    Mat img = imread("C:/Users/81702/image/021851.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayTempestdragon() {
    Mat img = imread("C:/Users/81702/image/023507.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}

void displayifrit() {
    Mat img = imread("C:/Users/81702/image/023205.jpg");

    // 画像が正しく読み込まれているかチェック
    if (img.empty()) {
        cout << "画像が読み込めませんでした" << endl;
        return;
    }

    // 画像を表示
    imshow("Image", img);
    // キー入力を待つ
    waitKey(0);

    destroyAllWindows();
}




void advanceHardStory(Hero& player, int depth = 0) {
    if (depth == 6) {
        player.Storyend();
        return;

    }

    int choice;//選択肢を管理する変数
    vector<vector<string>> storyChoices = {//二次元ベクターでストーリーの選択肢を管理
        {"家の中で使えそうなものを探す","冒険に出るために，門の方に行く","お金が必要なのでまじめに働く"},
        {"怪しい森を探検する","神秘的な森に進む","海辺で釣りをする"},
        {"滝裏の洞窟を探検する","迷路のようなダンジョンに入る","墓荒らしをする"},
        {"妖力に満ちた，湖に向かう","魔女の里に向かう","金を取りに鉱山へ向かう"},
        {"魔王の城へ進む","燃え上がる火山に向かう","天空の浮遊島に向かう"},
        {"自らが魔王となる","戦いつかれたので，Youtubeドリームを目指す","さらなる旅へ向かう"}


    };
    cout << "選択肢:\n";
    for (int i = 0; i < storyChoices[depth].size(); ++i) {
        cout << i + 1 << ". " << storyChoices[depth][i] << "\n";//二次元ベクターのdepth行i列成分を順に表示し，ストーリー分岐を提示
    }
    cout << "4.メニューを開く\n";

    cout << "キーボードで選択肢を選んでね:";
    cin >> choice;
    //Switch文の外に置くことで，毎選択肢(depthの進行度）ごとにメニュー表示の選択ができるようにした
    if (choice == 4) {
        handleMenu(player);
        advanceHardStory(player, depth);//メニュー選択後に元のストーリーの進行度合い(depth)に応じた選択肢の表示に戻れる
        return;
    }
    //ストーリーの進行
    switch (depth) {
    case 0:
        if (choice == 1) {
            cout << "何か使えそうなものはないか，半年ぶりに掃除した！\n";
            cout << "これとか，使えそうだぞ!\n";
            Weapon newWeapon("メリケンサック", 30);
            Armor newArmor("モコモコセーター", 7);
            player.equipWeapon(newWeapon);
            player.equipArmor(newArmor);
        }
        else if (choice == 2) {
            cout << "門番に，呼び止められた!\n" << "門番A:おい，どこに行くんだ？，冒険に出るつもりか？\n";
            cout << "このあたりのモンスターは強い，そんな装備で行くとは，なかなかロックだね，お古でいいなら，これを上げるよ\n";
            Weapon newWeapon("Japanese Sword(日本刀）", 25);
            Armor newArmor("Japanese Armor(甲冑）", 15);
            player.equipWeapon(newWeapon);
            player.equipArmor(newArmor);
            cout << "(ただの，日本コレクターじゃないか！世界観が，めちゃくちゃだ！）\n";
            cout << "コレクターな門番A:なあに，礼には及ばないさ,おじさんも若いころは，無茶をしたもんだ!\n";

        }
        else if (choice == 3) {
            cout << "お金がないから，まじめに働こう,今は,Youtuberが熱いらしい\n";
            cout << "Yotubeチャンネル「100日後に働くワイ」の収益化に成功した!\n";
            cout << "100000G　手に入れた\n";
            player.earnMoney(100000);
            cout << "これを元手にコロシアムで一発当てて，億万長者になるぞ\n" << "できるだけオッズが高いところに，かけた，ちきってるやつは，弱いのだ。\n";
            cout << "70000G 負けた!\n" << player.getName() << "は賢いので，この辺にしといてやったのだ()\n";
            player.spendMoney(70000);
        }
        break;

    case 1:
        if (choice == 1) {
            cout << "怪しいキノコを見つけた\n";
            HealingItem poison("怪しいキノコ", 5);
            player.addhealingitem(poison);
            cout << "怪しいキノコを見つけた，食べて大丈夫なのか?\n";
        }
        else if (choice == 2) {
            cout << "森を進むと,おっさんのエルフがいた,夢が，壊れてしまう,,,\n";
            HealingItem ElfMedicine("エルフの回復薬", 200);
            player.addhealingitem(ElfMedicine);
            cout << "エッチな本を渡したら,エルフの回復薬をくれた，これはとんでもなく効果がありそうだ\n";

        }
        else if (choice == 3) {
            cout << "海辺で釣りをていたら,ウシガエルが釣れた,どうやって釣り竿で釣ったのか\n";
            cout << "外来種なので,食べてやるのはきっといいことなのだ，ガハハハッ\n";
            HealingItem bullfrog("ウシガエル", 50);
            player.addhealingitem(bullfrog);
            displayRadioFish();
            cout << "魚も，釣りたいぞ\n" << "30分後～\n" << "魚モンスターが，釣れた，うーん，お前じゃない!!\n";
            cout << "RADIO FISHが襲い掛かってきた，聞いたことある，名前だぞ！\n";
            Enemy enemy("RADIO FISH", 50, 12);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "I'm a PERFECT HUMABN!!\n";
        }
        break;
    case 2:
        if (choice == 1) {
            displayOak();
            cout << "滝裏の洞窟を進むと，オークの巣だった，入り口の方に引きながら一体ずつ,倒していくしかない!\n";
            Enemy enemyA("オークA", 50, 10);
            player.battle(enemyA);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            Enemy enemyB("オークB", 55, 11);
            player.battle(enemyB);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            Enemy enemyC("オークC", 50, 13);
            player.battle(enemyC);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "オークの群れを倒した!\n";
            cout << "オークが盾を,落としたぞ!\n";
            Armor newArmor("オークの盾", 8);
            player.equipArmor(newArmor);

        }
        else if (choice == 2) {
            displayskeleton();
            cout << "迷路のように，入り組んでいる，何か出てきそう!\n";
            cout << "案の定，謎のモンスターに，襲い掛かられた！\n";
            Enemy enemy1("スケルトン", 70, 15);
            player.battle(enemy1);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            displaymimic();
            cout << "先に進むと，宝箱があった!\n" << "宝箱だと思ったら,ミミックだった!こいつは，手ごわいぞ\n";
            Enemy enemy2("ミミック", 150, 35);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "手ごわい相手だった,あれ，何か中に入っているぞ\n";
            Armor newArmor("ダンジョンの秘宝", 38);
            player.equipArmor(newArmor);
        }
        else if (choice == 3) {
            displayRiche();
            cout << "死人に口なし，お宝を，探すのだ\n" << "墓荒らしをしていると，背筋に冷気を感じた，生きたぬくもりを，感じられない．．．\n";
            cout << "アンデッドの王,リッチーが現れた!\n";
            Enemy enemy("リッチー:King of undead", 200, 9);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "アンデッドの王なだけあって，かなりしぶとかった...何か，落としたぞ!\n";
            cout << "不死の秘宝を手に入れた，これ，人間が使っても，大丈夫なの?\n";
            HealingItem undeadTreasure("不死の秘宝", 250);
            player.addhealingitem(undeadTreasure);
        }
        break;
    case 3:
        if (choice == 1) {
            cout << "薄暗い湖のほとりに来た,辺りは迷霧に満ちている\n";
            cout << "抜いちゃいけなさそうな，いかにもな大剣がある，どうしよう\n";
            cout << "1.思い切り引き抜く\n" << "2.怖いので抜かない，と見せかけてやっぱりそっと抜く\n";
            int switchselect;
            cin >> switchselect;
            if (switchselect == 1 || switchselect == 2) {
                cout << "大剣:十拳の剣を手に入れた！ラッキー！\n";
                Weapon newWeapon("十拳の剣", 88);
                player.equipWeapon(newWeapon);
                displayOrochi();
                cout << "ゴゴゴゴゴゴゴ.....";
                cout << "ヤマタノオロチが現れた，なんで抜いてしまったんだろう\n";
                cout << "ひよってる奴いる？いねえよなぁ!!(開き直り)\n";
                Enemy enemy("ヤマタノオロチ", 888, 55);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//プレイヤーが死んだときはここで終了
                }
            }
            cout << "ちびるかと思った,,,,ちょっと休憩しよう...\n";
            cout << "湖のほとりで，何かアイテムを見つけた!これは何だ?(メニューからみられるヨ!)\n";
            cout << "何か，神々しい容器のようなものだ\n";
            HealingItem undeadTreasure("アマテラスの盃", 230);
            player.addhealingitem(undeadTreasure);

        }
        else if (choice == 2) {
            cout << "魔女の里に行くと，魔女長のところに，案内された，何ともきれいな人であるっ\n";
            cout << "なんでも，里の最年長らしい，これが，美魔女というやつか\n";
            cout << "美魔女:お前さんに，魔法の使い方を教えてあげるよ，セミナー，今なら初回限定割引，30000Gでどうだい？\n";
            cout << "美人局だったみたいだ,,,\n";
            cout << "1.購入する\n" << "2.購入しない\n";
            int selectbuy;
            cin >> selectbuy;
            if (selectbuy == 1) {
                if (player.getMoney() >= 30000) {
                    player.spendMoney(30000);
                    cout << "30000G払った．\n";
                    Weapon newWeapon("○ワトコの杖", 75);
                    Armor newArmor("魔女のローブ（男用）", 30);
                    player.equipWeapon(newWeapon);
                    player.equipArmor(newArmor);
                }
                else if (player.getMoney() < 30000) {
                    cout << "お金が足りなかった…\n";
                }
            }
            else if (selectbuy == 2) {
                cout << "買うのは，やめておこう…（美人局にはかかわるな）\n";
                cout << "あたりを散策してみた!\n";
                displayhomunculus();
                cout << "魔女の里にいた,魔女のホムンクルスと戦った\n";
                Enemy enemy("ホムンクルス", 135, 10);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//プレイヤーが死んだときはここで終了
                }
            }
            cout << "美人局:ここから先は，さらに強いモンスターたちだよ!頑張っておいで!\n";
            cout << "魔女の薬をもらった\n";
            HealingItem witchmedicine("魔女の薬", 99);
            player.addhealingitem(witchmedicine);
        }
        else if (choice == 3) {
            cout << "鉱山の中には，色々な鉱石がたくさんだ\n";
            cout << "金を掘って，装備を作った!\n";
            Weapon newWeapon("黄金Sword", 70);
            player.equipWeapon(newWeapon);
            displaygolem();
            cout << "欲張って夢中で掘っていたら,ゴーレムに襲われた\n";
            Enemy enemy("ゴーレム", 500, 45);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "ゴーレムを乱獲して売ったら，すごい金になりそうだ!\n";
            cout << "GETした金やゴーレムを売って，回復薬をしこたま購入した\n";
            HealingItem megapotion("メガポーション", 150);
            player.addhealingitem(megapotion);
            HealingItem medicine("傷薬", 80);
            player.addhealingitem(medicine);
        }
        break;
    case 4:
        if (choice == 1) {
            if (player.getInventory().empty()) {
                cout << "魔王の城来たというのに，俺は何の武器も防具も持っていない....\n";
                cout << "信じられるものは，己の拳のみだ「拳」で!\n";
                cout << player.getName() << "は覚醒し，武闘家にクラスチェンジした!(してません）\n";
                Weapon newWeapon("己の拳", 9999);//武器を持たないルート(1ルートのみ）を選択した場合，ここで最強武器をGET
                player.equipWeapon(newWeapon);
            }
            displayCerberus();
            cout << "魔王の城に入った...あたりは薄暗く,壁を伝って道なりに進んでいく\n";
            cout << "魔王の宝物庫を見つけた!，物色していると，ケルベロスが襲い掛かってきた\n";
            Enemy enemy1("ケルベロス", 999, 45);
            player.battle(enemy1);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "宝物庫から，アイテムを手に入れた\n";
            Weapon newWeapon("ロ○の剣", 150);
            player.equipWeapon(newWeapon);
            Armor newArmor("○トの鎧", 75);
            player.equipArmor(newArmor);
            HealingItem HyperPotion("ハイパーポーション", 350);
            player.addhealingitem(HyperPotion);
            player.usehealingItem();
            displaymaou();
            cout << "魔王(Hard)が現れた，Hardなので強そうだ\n";
            cout << "よく来たな" << player.getName() << "よ，ここでくたばるがいい!!\n";
            Enemy enemy2("魔王(Hard)", 1230, 115);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }


        }
        else if (choice == 2) {
            cout << "燃え盛る活火山についた，灼熱の空気があたりにたちこめている\n";
            cout << "火口付近に，何か巨大なモンスターを見つけた!\n";
            displayifrit();
            cout << "灼熱の魔神，イフリートと目があってしまった\n";
            Enemy enemy("イフリート", 500, 84);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "攻撃がとても痛い，手ごわい相手だった\n";
            cout << "火口のイフリートがいた場所に，何か見つけた!\n";
            Weapon newWeapon("魔剣イフリート", 175);
            player.equipWeapon(newWeapon);
        }
        else if (choice == 3) {
            cout << "魔女の箒に乗って，天空の浮遊島に向かった\n";
            displayTempestdragon();
            cout << "はるか上空にそびえたつ神殿には，強大なドラゴンが佇んでいた!\n";
            Enemy enemy("テンペストドラゴン", 1050, 51);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//プレイヤーが死んだときはここで終了
            }
            cout << "浮遊島には，空の民がいた!世界は広い!\n";
            cout << "「ドラゴンスレイヤー」の称号を手に入れた!\n";
        }
        break;

    case 5:
        if (choice == 1) {
            cout << player.getName() << "は，魔王の玉座に座り，世界征服に向けて動き始めた!\n";
            cout << "人の夢は!!!終わらねェ!!!\n";
        }
        else if (choice == 2) {
            cout << "Youtuberとして，名をはせた!!\n";
            cout << "5000000G 手に入れた!!\n";
            player.earnMoney(5000000);


        }
        else if (choice == 3) {
            cout << "俺の戦いは，まだまだ終わらない!!\n";
            cout << "(Hardモードでは，あるルートを通ると，秘密の最強武器がGETできるよう!探してみてね!)\n";
        }
        break;

    }

    advanceHardStory(player, depth + 1); //次のステージへ進む

}

int main() {
    srand(time(0));
    cout << "遊びかた:表示された選択肢の中で，番号を選んで，ストーリーを進めて行ってね!\n";
    cout << "敵キャラの画像が表示された時は，×ボタン，若しくはキーボードを押せば，ストーリーが進むよ\n";
    //プレイヤーの名前を決めてもらう
    string Heroname;
    cout << "勇者の名前をキーボードで入力してね！: ";
    cin >> Heroname;
    //プレイヤーを作成（名前をユーザー入力に変更）
    Hero hero(Heroname, 150, 50, Weapon("素手", 3), Armor("私服", 1));
    cout << "ここからあなたの冒険が始まります．難易度を選んで番号をキーボードで入力してね\n" << "1.Normal\n" << "2.Hard\n";
    int select;
    bool validselection = false;
    while (!validselection) {//間違った入力はもう一度選択肢提示される
        cin >> select;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(100, '\n');//数字以外の入力を無視して再度選択肢提示
            cout << "無効な選択肢だよ!1～2の間で再入力してね\n";
        }
        else {
            if (select == 1) {
                advanceNormalStory(hero);
                validselection = true;
            }
            if (select == 2) {
                advanceHardStory(hero);
                validselection = true;
            }
            else if (select < 1 || 2 < select) {
                cout << "無効な選択肢だよ!1～2の間で再入力してね\n";//1～2以外の数字の入力は,再度選択肢提示
            }
        }
    }
}

