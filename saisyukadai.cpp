#include<iostream>
#include<string>
#include<vector>
#include<ctime>�@//������o�ߎ��Ԃ̎擾�Ɏg�p
#include<cstdlib> //rand�֐��̎g�p�Ɏg�����D
#include <opencv2/opencv.hpp> //�摜�\���̂��߂̊O�����C�u�����i��ʌ��J����Ă�����́j
using namespace cv;
using namespace std;

//Item���Ǘ�����N���X
class Item {
protected:
    string name;
public:
    Item(string n) : name(n) {}
    string getName()const { return name; }
};

//����̃N���X�i�U���͂��Ǘ��j
class Weapon : public Item {
private:
    int attackPower;
public:
    Weapon(string n, int ap) : Item(n), attackPower(ap) {}//�R���X�g���N�^���`
    int getattackPower()const { return attackPower; }
};

// �h��N���X�i��_���[�W���Ǘ��j
class Armor : public Item {
private:
    int defensePower;  // �h���
public:
    Armor(string n, int dp) : Item(n), defensePower(dp) {}
    int getDefensePower() const { return defensePower; }
};

//�񕜃A�C�e���̃N���X�i�񕜗ʂ��Ǘ��j
class HealingItem : public Item {
private:
    int healingPower;
public:
    HealingItem(string n, int h) : Item(n), healingPower(h) {} //�R���X�g���N�^���`
    int gethealingPower() const { return healingPower; }
};

//Player��Enemy�̋��ʃN���X
class Common {
protected:
    string name;
    int HP;
public:
    Common(string n, int hp) : name(n), HP(hp) {}
    //�_���[�W�v�Z�̊֐��@
    virtual void  takedamage(int damage) { HP -= damage; };
    //�������m�F����֐��i�l��Ԃ��ď����j
    virtual bool stillAlive() const { return HP > 0; };
    virtual int getBaseAttackPower() const { return 0; }//��b�U���͂��擾����֐�
    string getName()const { return name; }//���O�擾�֐�

};

//Player �N���X
class Hero : public Common {
private:
    int money;
    Weapon weapon;
    Armor armor;
    vector<Item> Inventory;  //Item���Ǘ�����x�N�^�[
    vector<HealingItem> healingItems; //healingItem���Ǘ�����x�N�^�[
    time_t startTime;//�Q�|���̊J�n�������N���X�\�z���Ɏ擾
public:
    Hero(string n, int hp, int m, Weapon w, Armor a) : Common(n, hp), money(m), weapon(w), armor(a)
    {
        startTime = time(nullptr);   //�Q�[���̊J�n���Ԃ��瑪��
    }

    int getMoney() const { return money; }

    void earnMoney(int amount) {
        money += amount;
    }
    void spendMoney(int amount) {
        money -= amount;
    }

    //�X�e�[�^�X�m�F�֐�
    void showStatus() {
        cout << "���O:" << name << "\nHP:" << HP << "\n������" << money << "\n����:" << weapon.getName() << "\n�U����"
            << weapon.getattackPower() << "\n";
        cout << "�h��:" << armor.getName() << "\n�h���:" << armor.getDefensePower() << "\n";
        displayInventory();
        displayhealingItems();
    };

    //�A�C�e����Inventory�x�N�^�[�ɒǉ�
    void addItem(const Item& item) {
        Inventory.push_back(item);
    }
    //�񕜃A�C�e����healingItems�x�N�^�[�ɒǉ�
    void addhealingitem(const HealingItem& healingitem) {
        healingItems.push_back(healingitem);
    }
    //�C���x���g�����C�e���[�^�[��p���ĕ\������֐�
    void displayInventory() {
        cout << "�l���A�C�e���i����C�h��j:";
        for (auto it = Inventory.begin();it != Inventory.end(); it++) {
            cout << it->getName() << " ";
        }
        cout << endl;
    }
    //�񕜃A�C�e�����C�e���[�^�[��p���ĕ\������֐�
    void displayhealingItems() {
        cout << "�l�������񕜃A�C�e��:";
        for (auto it = healingItems.begin();it != healingItems.end(); it++) {
            cout << it->getName() << " ";
        }
        cout << endl;
    }
    //Common�N���X����I�[�o�[���C�h
    void takedamage(int damage) override {
        HP -= damage;
    }

    bool stillAlive() const override { return HP > 0; }//�����m�F�֐�

    //HP���񕜂���֐�
    void heal(int amount) {
        HP += amount;
        if (HP > 350) {//HP�̏����300�ɐݒ�
            HP = 350;
        }
    }

    //�G�Ƃ̐퓬���s���֐�
    void battle(Common& enemy) {
        srand(time(0));//�����̒l��������

        while (stillAlive() && enemy.stillAlive()) {//�������G�����ʂ܂Ő퓬�p��
            int baseDamage = rand() % 10 + 1;//��{�_���[�W��10�`�P�̊ԂŃ����_���Ɍ���
            int playerDamage = baseDamage + weapon.getattackPower();//����̍U���͂����Z
            int enemyBaseDamage = rand() % 10 + 1;//�G�̃_���[�W�ɗ�����10�`�P�̊Ԃ̃u����t����
            int enemyDamage = enemyBaseDamage + enemy.getBaseAttackPower();//�G�̊�b�U���͂����Z�i�����X�^�[���ƂɌ���j
            //�v���C���[�̍U��
            cout << "���Ȃ��̍U���I" << weapon.getName() << "�œG���U������I�@�_���[�W: " << playerDamage << "\n";
            enemy.takedamage(playerDamage);//�G���_���[�W���󂯂�
            if (!enemy.stillAlive()) {
                cout << enemy.getName() << "��|�����I\n";
                break;
            }
            //�h��̖h��͕��_���[�W���y��
            int reducedDamage = enemyDamage - armor.getDefensePower();
            if (reducedDamage < 0) {
                reducedDamage = 0;
            }
            //�G�̍U��
            cout << "�G�̍U���I�_���[�W: " << reducedDamage << "\n";
            takedamage(reducedDamage); //�v���C���[���_���[�W���󂯂�
            cout << "�c���HP" << HP << "\n";
            if (!stillAlive()) {
                cout << "�Q�[���I�[�o�[\n";
            }
        }
    }

    //�v���C���Ԃ�\�����邽�߂̊֐�
    void showPlayTime() const {
        //���ݎ����̎擾
        time_t currentTime = time(nullptr);
        //�v���C���Ԃ̌v�Z
        double playTime = difftime(currentTime, startTime);
        cout << "�V�񂾎���" << playTime << "�b\n";
    }
    //����𑕔����邽�߂̊֐�(�������𑕔��j
    void equipWeapon(const Weapon& newWeapon) {
        if (newWeapon.getattackPower() >= weapon.getattackPower()) {
            weapon = newWeapon;
            cout << "�V��������:" << weapon.getName() << "�𑕔������I\n";
            //�V����������C���x���g���ɒǉ�
            addItem(newWeapon);
        }
        else {
            cout << "�c�O!�������Ă��镐��̕����C��������!\n";
        }
    }
    //�h��𑕔����邽�߂̊֐��i�������𑕔��j
    void equipArmor(Armor& newArmor) {
        if (newArmor.getDefensePower() >= armor.getDefensePower()) {
            armor = newArmor;
            cout << "�V�����h��:" << armor.getName() << "�𑕔������I\n";
            addItem(newArmor);
        }
        else {
            cout << "�c�O!�������Ă���h��̕����C��������!\n";
        }
    }
    //�񕜃A�C�e�����g�p���邽�߂̊֐�
    void usehealingItem() {
        if (!healingItems.empty()) {//�񕜃A�C�e�����������Ă��邩�̊m�F 
            HealingItem healingItem = healingItems.back();
            healingItems.pop_back();
            heal(healingItem.gethealingPower());   //heal�֐���HP����
            cout << healingItem.getName() << "���g�p���āCHP��" << healingItem.gethealingPower() << "�񕜂��܂���\n";
            cout << "���݂�HP:" << HP << "\n";

        }
        else {
            cout << "�񕜃A�C�e���������Ă��܂���I\n";
        }
    }
    //�C���x���g���x�N�^�[���擾
    const vector<Item>& getInventory() const {
        return Inventory;
    }

    //�I���q���I���C�Q�[�����I������Ƃ��ɕ\��
    void Storyend() {
        cout << "�`����,,,�I�����,,,\n";
        showStatus();
        showPlayTime();
        cout << "�V��ł���Ă��肪�Ƃ�!�ʃ��[�g�����邩��V��ł݂ĂˁI\n";
    }

};


//Enemy�N���X�F�G�̏��Ǘ�
class Enemy : public Common {
private:
    int baseattackPower; //�G�̊�b�U���͂��w��
public:
    Enemy(string n, int hp, int ap) : Common(n, hp), baseattackPower(ap) {}
    //�_���[�W���󂯂�֐�(override)
    void takedamage(int damage) override { HP -= damage; }
    //�����m�F�֐�
    bool stillAlive() const override { return HP > 0; }
    int getBaseAttackPower() const override { return baseattackPower; }
};

// ���j���[�\���֐�
void displayMenu() {
    cout << "1. �X�e�[�^�X�m�F\n";
    cout << "2. �`���ɖ߂�\n";
    cout << "3. �񕜃A�C�e�����g��\n";
    cout << "4. �Q�[�����I������\n";
}

// ���j���[����֐�
void handleMenu(Hero& player) {
    int menuChoice;//���j���[�̑I�������Ǘ�����ϐ�
    bool inMenu = true; //true�̊ԊJ��������
    while (inMenu) {
        displayMenu();//�I������I�΂���
        cout << "�I��������͂��Ă�������: ";
        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            player.showStatus();
            break;
        case 2:
            inMenu = false;//���j���[�����
            break;
        case 3:
            player.usehealingItem();
            break;
        case 4:
            player.Storyend();
            exit(0);
        default:
            cout << "�����ȑI�����ł��B�ēx�I�����Ă��������B\n";
        }
    }
}


//�����X�^�[�̉摜�\���̂��߂̊֐�
void displaySliym() {
    Mat img = imread("C:/Users/81702/image/183818.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaybat() {
    Mat img = imread("C:/Users/81702/image/185028.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaySeaCucumer() {
    Mat img = imread("C:/Users/81702/image/190719.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayghost() {
    Mat img = imread("C:/Users/81702/image/191254.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaysubordinate() {
    Mat img = imread("C:/Users/81702/image/192953.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaymaoueasy() {
    Mat img = imread("C:/Users/81702/image/maou.png");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayDoragon() {
    Mat img = imread("C:/Users/81702/image/200926.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayDoragon() {
    Mat img = imread("C:/Users/81702/image/200926.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayarchDemon() {
    Mat img = imread("C:/Users/81702/image/202012.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

//�I�����X�g�[���[��W�J���邽�߂̊֐�
void advanceNormalStory(Hero& player, int depth = 0) {// depth�ɂ���ăX�g�[���[�̐[�����Ǘ�(Storyend��depth�͕���̍s�̐��{1)
    if (depth == 6) {
        player.Storyend();
        return;

    }
    int choice;//�I�������Ǘ�����ϐ�
    vector<vector<string>> storyChoices = {//�񎟌��x�N�^�[�ŃX�g�[���[�̑I�������Ǘ�
        {"���X�̎�`��������","�ӂ���U�􂷂�","�߂��ɐl������̂Řb��"},
        {"�`���ɏo��","�󔠂�T��","�ʂ���Ȃ����ĕ��퉮��T��"},
        {"���A�ɓ���","�X�̒��ɓ���","�C�̕��ɍs��"},
        {"�H����T��","�Â��قɓ���","��n���U�􂷂�" },
        {"�����Ɛ키","�h���S���Ɛ키","�f�[�����Ɛ키"},
        {"�̋��ɖ߂��Ă̂�т��炷","����苭�����T���ɍs��","���l�ɂȂ��ĉ҂�"}


    };
    cout << "�I����:\n";
    for (int i = 0; i < storyChoices[depth].size(); ++i) {
        cout << i + 1 << ". " << storyChoices[depth][i] << "\n";//�񎟌��x�N�^�[��depth�si�񐬕������ɕ\�����C�X�g�[���[������
    }
    cout << "4.���j���[���J��\n";

    cout << "�L�[�{�[�h�őI������I��ł�:";
    cin >> choice;
    //Switch���̊O�ɒu�����ƂŁC���I����(depth�̐i�s�x�j���ƂɃ��j���[�\���̑I�����ł���悤�ɂ���
    if (choice == 4) {
        handleMenu(player);
        advanceNormalStory(player, depth);//���j���[�I����Ɍ��̃X�g�[���[�̐i�s�x����(depth)�ɉ������I�����̕\���ɖ߂��
        return;
    }

    //�X�g�[���[�̐i�s
    switch (depth) {
    case 0:
        if (choice == 1) {
            "�o��̏���:�`�����̂Ȃ񂾂̌����ĂȂ��ł������ƒ�E�����Ȃ�����\n";
            player.earnMoney(100);
            cout << "100G��ɓ��ꂽ!\n";
        }
        else if (choice == 2) {
            cout << "��������U�􂷂�ƁC������������\n";
            HealingItem potion("����", 20);
            player.addhealingitem(potion);
            cout << "�����������\n";
        }
        else if (choice == 3) {
            cout << "���o����ɘb��������!\n"
                << "���o����:����������Ă��Ȃ��悤������,�`���͏��߂āH���̂�����̃����X�^�[�͎ア���ǁC����������Ē��񂾕����������\n"
                << "�����̒ʂ���Ȃ������Ƃ���ɕ��퉮�����邩��C�����čs���Ƃ������!\n";
        }
        break;

    case 1:
        if (choice == 1) {
            displaySliym();
            cout << "�X���C���������I�킨���I\n";
            Enemy enemy("�X���C��", 10, 3);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
        }
        else if (choice == 2) {
            cout << "�����ȕ󔠂��J�����I���ɂ�" << "�؂̖_�������Ă���\n";
            Weapon newWeapon("�؂̖_", 5);
            player.equipWeapon(newWeapon);
        }
        else if (choice == 3) {
            cout << "���퉮��������\n" << "���l:���܂Ȃ畐��Ɩh��Z�b�g��,100G���悤!\n";
            cout << "��������" << player.getMoney() << "�ł�.\n";
            cout << "1.�w������\n" << "2.�~�߂Ă���\n";
            int selectbuy;
            cin >> selectbuy;
            if (selectbuy == 1) {
                if (player.getMoney() >= 100) {
                    player.spendMoney(100);
                    cout << "100G�������D\n";
                    Weapon newWeapon("�S�̌�", 15);
                    Armor newArmor("�S�̊Z", 5);
                    player.equipWeapon(newWeapon);
                    player.equipArmor(newArmor);
                }
                else if (player.getMoney() < 100) {
                    cout << "����������Ȃ������c\n";
                }
            }
            else if (selectbuy == 2) {
                cout << "�����̂́C��߂Ă������c�i��������j\n";
            }
        }
        break;
    case 2:
        if (choice == 1) {
            displaybat();
            cout << "���A�̒��ɂ́C�댯�ȃR�E�������������񂢂�c\n"
                << "�f���W�����X�R�E�������C�P���|�����Ă���\n";
            Enemy enemy("�f���W�����X�R�E����", 30, 5);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
        }
        else if (choice == 2) {
            HealingItem ElfMedicine("�G���t�̉񕜖�", 80);
            player.addhealingitem(ElfMedicine);
            cout << "�Ȃ�ƁC�X�̒��ɂ̓G���t������\n" << "�G���t�̉񕜖�������C����͂��������ʂ����肻����\n";
        }
        else if (choice == 3) {
            displaySeaCucumer();
            cout << "�C�̐󐣂ɁC�f���W�����X�i�}�R���o������\n";
            Enemy enemy("�f���W�����X�i�}�R", 40, 4);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�f���W�����X�i�}�R��|������C�������Ƃ�����!\n";
            Armor newArmor("�i�}�R�A�[�}�[", 8);
            player.equipArmor(newArmor);

        }
        break;
    case 3:
        if (choice == 1) {
            cout << "�؂̏��T���ĐH����������\n";
            HealingItem mysteriousfruit("��̖؂̎�", 15);
            player.addhealingitem(mysteriousfruit);
            cout << "��̖؂̎���������\n";
        }
        else if (choice == 2) {
            displayghost();
            cout << "�ق̒��́C�^���Â�\n";
            cout << "�����Ă��������ŁC�����������\n";
            cout << "�����[������b���o������\n";
            Enemy enemy("�����[������b", 50, 7);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
        }
        else if (choice == 3) {
            cout << "��r�炵�����Ă����,������������\n";
            cout << "�􂢂̃����O��������\n";
            Weapon newWeapon("�􂢂̃����O", 25);
            player.equipWeapon(newWeapon);
        }
        break;
    case 4:
        if (choice == 1) {
            cout << "�����̏�ցC�˓�����\n";
            displaysubordinate();
            cout << "�����̉����[�����ꂽ\n";
            Enemy enemy("�����̉����[", 50, 6);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�����̉����[��|����\n" << "�����̑҂ʍ���,�꒼����\n";
            displaymaoueasy();
            cout << "����(Normal)�����ꂽ�CNormal�Ȃ̂Ŏセ����\n";
            cout << "�悭������" << player.getName() << "��C�����ł����΂邪����!!\n";
            Enemy enemy2("����(Normal)", 100, 10);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "������|����������C����Ȃ��̂�,,,\n";
        }
        else if (choice == 2) {
            cout << "���̏Z�ގR�Ɍ�������\n"
                << "�������h���S��������,�ߊl���悤\n";
            displayDoragon();
            cout << "�w��ɍ����e��!!��h���S�������ł���\n" << "��͋����C�키�����Ȃ�!\n";
            Enemy enemy("��h���S��", 95, 12);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "��h���S����|�����C�Ă����点�₪���āC���̃K�L�͍������ꂻ������\n";
        }
        else if (choice == 3) {
            cout << "���Â��炠��C�����̈�Ղɂ���Ă���\n"
                << "�s�C���ȕ��l�̉���,�����C��Ȃ����ȐԂ��X�C�b�`������C�����܂���\n"
                << "1.����\n" << "2.����\n";
            int switchselect;
            cin >> switchselect;
            if (switchselect == 1 || switchselect == 2) {
                displayarchDemon();
                cout << "�A�[�N�f�[���������ꂽ�C�Ȃ�ŃX�C�b�`������������񂾂낤\n";
                Enemy enemy("�A�[�N�f�[����", 93, 10);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
                }
            }

        }
        break;

    case 5:
        if (choice == 1) {
            cout << player.getName() << "�́C�T�܂��������ۂ��炵��\n";
        }
        else if (choice == 2) {
            cout << "����ɋ�������Ƃ�肽���Ȃ�CHard���[�h���������߂���!\n";
        }
        else if (choice == 3) {
            cout << player.getName() << "�ɂ́C���˂��������I�Ȃ�Ə��l�̕��������Ă����̂�,�ɂ����j�߁I\n";
            cout << "1000000G,�҂���\n";
            player.earnMoney(1000000);
        }
        break;

    }
    advanceNormalStory(player, depth + 1); //���̃X�e�[�W�֐i��
}



void displayRadioFish() {
    Mat img = imread("C:/Users/81702/image/224829.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayOak() {
    Mat img = imread("C:/Users/81702/image/231311.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayskeleton() {
    Mat img = imread("C:/Users/81702/image/232133.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaymimic() {
    Mat img = imread("C:/Users/81702/image/234222.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayRiche() {
    Mat img = imread("C:/Users/81702/image/002325.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayOrochi() {
    Mat img = imread("C:/Users/81702/image/010216.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayhomunculus() {
    Mat img = imread("C:/Users/81702/image/013133.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displaygolem() {
    Mat img = imread("C:/Users/81702/image/015508.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayCerberus() {
    Mat img = imread("C:/Users/81702/image/022809.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}
void displaymaou() {
    Mat img = imread("C:/Users/81702/image/021851.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayTempestdragon() {
    Mat img = imread("C:/Users/81702/image/023507.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}

void displayifrit() {
    Mat img = imread("C:/Users/81702/image/023205.jpg");

    // �摜���������ǂݍ��܂�Ă��邩�`�F�b�N
    if (img.empty()) {
        cout << "�摜���ǂݍ��߂܂���ł���" << endl;
        return;
    }

    // �摜��\��
    imshow("Image", img);
    // �L�[���͂�҂�
    waitKey(0);

    destroyAllWindows();
}




void advanceHardStory(Hero& player, int depth = 0) {
    if (depth == 6) {
        player.Storyend();
        return;

    }

    int choice;//�I�������Ǘ�����ϐ�
    vector<vector<string>> storyChoices = {//�񎟌��x�N�^�[�ŃX�g�[���[�̑I�������Ǘ�
        {"�Ƃ̒��Ŏg�������Ȃ��̂�T��","�`���ɏo�邽�߂ɁC��̕��ɍs��","�������K�v�Ȃ̂ł܂��߂ɓ���"},
        {"�������X��T������","�_��I�ȐX�ɐi��","�C�ӂŒނ������"},
        {"�ꗠ�̓��A��T������","���H�̂悤�ȃ_���W�����ɓ���","��r�炵������"},
        {"�d�͂ɖ������C�΂Ɍ�����","�����̗��Ɍ�����","�������ɍz�R�֌�����"},
        {"�����̏�֐i��","�R���オ��ΎR�Ɍ�����","�V��̕��V���Ɍ�����"},
        {"���炪�����ƂȂ�","�킢���ꂽ�̂ŁCYoutube�h���[����ڎw��","����Ȃ闷�֌�����"}


    };
    cout << "�I����:\n";
    for (int i = 0; i < storyChoices[depth].size(); ++i) {
        cout << i + 1 << ". " << storyChoices[depth][i] << "\n";//�񎟌��x�N�^�[��depth�si�񐬕������ɕ\�����C�X�g�[���[������
    }
    cout << "4.���j���[���J��\n";

    cout << "�L�[�{�[�h�őI������I��ł�:";
    cin >> choice;
    //Switch���̊O�ɒu�����ƂŁC���I����(depth�̐i�s�x�j���ƂɃ��j���[�\���̑I�����ł���悤�ɂ���
    if (choice == 4) {
        handleMenu(player);
        advanceHardStory(player, depth);//���j���[�I����Ɍ��̃X�g�[���[�̐i�s�x����(depth)�ɉ������I�����̕\���ɖ߂��
        return;
    }
    //�X�g�[���[�̐i�s
    switch (depth) {
    case 0:
        if (choice == 1) {
            cout << "�����g�������Ȃ��̂͂Ȃ����C���N�Ԃ�ɑ|�������I\n";
            cout << "����Ƃ��C�g����������!\n";
            Weapon newWeapon("�����P���T�b�N", 30);
            Armor newArmor("���R���R�Z�[�^�[", 7);
            player.equipWeapon(newWeapon);
            player.equipArmor(newArmor);
        }
        else if (choice == 2) {
            cout << "��ԂɁC�Ăю~�߂�ꂽ!\n" << "���A:�����C�ǂ��ɍs���񂾁H�C�`���ɏo����肩�H\n";
            cout << "���̂�����̃����X�^�[�͋����C����ȑ����ōs���Ƃ́C�Ȃ��Ȃ����b�N���ˁC���Âł����Ȃ�C������グ���\n";
            Weapon newWeapon("Japanese Sword(���{���j", 25);
            Armor newArmor("Japanese Armor(�b�h�j", 15);
            player.equipWeapon(newWeapon);
            player.equipArmor(newArmor);
            cout << "(�����́C���{�R���N�^�[����Ȃ����I���E�ς��C�߂��Ⴍ���Ⴞ�I�j\n";
            cout << "�R���N�^�[�Ȗ��A:�Ȃ��ɁC��ɂ͋y�΂Ȃ���,����������Ⴂ����́C��������������!\n";

        }
        else if (choice == 3) {
            cout << "�������Ȃ�����C�܂��߂ɓ�����,����,Youtuber���M���炵��\n";
            cout << "Yotube�`�����l���u100����ɓ������C�v�̎��v���ɐ�������!\n";
            cout << "100000G�@��ɓ��ꂽ\n";
            player.earnMoney(100000);
            cout << "���������ɃR���V�A���ňꔭ���ĂāC�������҂ɂȂ邼\n" << "�ł��邾���I�b�Y�������Ƃ���ɁC�������C�������Ă��́C�ア�̂��B\n";
            cout << "70000G ������!\n" << player.getName() << "�͌����̂ŁC���̕ӂɂ��Ƃ��Ă�����̂�()\n";
            player.spendMoney(70000);
        }
        break;

    case 1:
        if (choice == 1) {
            cout << "�������L�m�R��������\n";
            HealingItem poison("�������L�m�R", 5);
            player.addhealingitem(poison);
            cout << "�������L�m�R���������C�H�ׂđ��v�Ȃ̂�?\n";
        }
        else if (choice == 2) {
            cout << "�X��i�ނ�,��������̃G���t������,�����C���Ă��܂�,,,\n";
            HealingItem ElfMedicine("�G���t�̉񕜖�", 200);
            player.addhealingitem(ElfMedicine);
            cout << "�G�b�`�Ȗ{��n������,�G���t�̉񕜖�����ꂽ�C����͂Ƃ�ł��Ȃ����ʂ����肻����\n";

        }
        else if (choice == 3) {
            cout << "�C�ӂŒނ���Ă�����,�E�V�K�G�����ނꂽ,�ǂ�����Ēނ�ƂŒނ����̂�\n";
            cout << "�O����Ȃ̂�,�H�ׂĂ��̂͂����Ƃ������ƂȂ̂��C�K�n�n�n�b\n";
            HealingItem bullfrog("�E�V�K�G��", 50);
            player.addhealingitem(bullfrog);
            displayRadioFish();
            cout << "�����C�ނ肽����\n" << "30����`\n" << "�������X�^�[���C�ނꂽ�C���[��C���O����Ȃ�!!\n";
            cout << "RADIO FISH���P���|�����Ă����C���������Ƃ���C���O�����I\n";
            Enemy enemy("RADIO FISH", 50, 12);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "I'm a PERFECT HUMABN!!\n";
        }
        break;
    case 2:
        if (choice == 1) {
            displayOak();
            cout << "�ꗠ�̓��A��i�ނƁC�I�[�N�̑��������C������̕��Ɉ����Ȃ����̂���,�|���Ă��������Ȃ�!\n";
            Enemy enemyA("�I�[�NA", 50, 10);
            player.battle(enemyA);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            Enemy enemyB("�I�[�NB", 55, 11);
            player.battle(enemyB);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            Enemy enemyC("�I�[�NC", 50, 13);
            player.battle(enemyC);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�I�[�N�̌Q���|����!\n";
            cout << "�I�[�N������,���Ƃ�����!\n";
            Armor newArmor("�I�[�N�̏�", 8);
            player.equipArmor(newArmor);

        }
        else if (choice == 2) {
            displayskeleton();
            cout << "���H�̂悤�ɁC����g��ł���C�����o�Ă�����!\n";
            cout << "�Ă̒�C��̃����X�^�[�ɁC�P���|����ꂽ�I\n";
            Enemy enemy1("�X�P���g��", 70, 15);
            player.battle(enemy1);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            displaymimic();
            cout << "��ɐi�ނƁC�󔠂�������!\n" << "�󔠂��Ǝv������,�~�~�b�N������!�����́C�育�킢��\n";
            Enemy enemy2("�~�~�b�N", 150, 35);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�育�킢���肾����,����C�������ɓ����Ă��邼\n";
            Armor newArmor("�_���W�����̔��", 38);
            player.equipArmor(newArmor);
        }
        else if (choice == 3) {
            displayRiche();
            cout << "���l�Ɍ��Ȃ��C������C�T���̂�\n" << "��r�炵�����Ă���ƁC�w�؂ɗ�C���������C�������ʂ�������C�������Ȃ��D�D�D\n";
            cout << "�A���f�b�h�̉�,���b�`�[�����ꂽ!\n";
            Enemy enemy("���b�`�[:King of undead", 200, 9);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�A���f�b�h�̉��Ȃ��������āC���Ȃ肵�ԂƂ�����...�����C���Ƃ�����!\n";
            cout << "�s���̔�����ɓ��ꂽ�C����C�l�Ԃ��g���Ă��C���v�Ȃ�?\n";
            HealingItem undeadTreasure("�s���̔��", 250);
            player.addhealingitem(undeadTreasure);
        }
        break;
    case 3:
        if (choice == 1) {
            cout << "���Â��΂̂قƂ�ɗ���,�ӂ�͖����ɖ����Ă���\n";
            cout << "�������Ⴂ���Ȃ������ȁC�����ɂ��ȑ匕������C�ǂ����悤\n";
            cout << "1.�v���؂��������\n" << "2.�|���̂Ŕ����Ȃ��C�ƌ��������Ă���ς肻���Ɣ���\n";
            int switchselect;
            cin >> switchselect;
            if (switchselect == 1 || switchselect == 2) {
                cout << "�匕:�\���̌�����ɓ��ꂽ�I���b�L�[�I\n";
                Weapon newWeapon("�\���̌�", 88);
                player.equipWeapon(newWeapon);
                displayOrochi();
                cout << "�S�S�S�S�S�S�S.....";
                cout << "���}�^�m�I���`�����ꂽ�C�Ȃ�Ŕ����Ă��܂����񂾂낤\n";
                cout << "�Ђ���Ă�z����H���˂���Ȃ�!!(�J������)\n";
                Enemy enemy("���}�^�m�I���`", 888, 55);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
                }
            }
            cout << "���т邩�Ǝv����,,,,������Ƌx�e���悤...\n";
            cout << "�΂̂قƂ�ŁC�����A�C�e����������!����͉���?(���j���[����݂��郈!)\n";
            cout << "�����C�_�X�����e��̂悤�Ȃ��̂�\n";
            HealingItem undeadTreasure("�A�}�e���X�̔u", 230);
            player.addhealingitem(undeadTreasure);

        }
        else if (choice == 2) {
            cout << "�����̗��ɍs���ƁC�������̂Ƃ���ɁC�ē����ꂽ�C���Ƃ����ꂢ�Ȑl�ł����\n";
            cout << "�Ȃ�ł��C���̍ŔN���炵���C���ꂪ�C�������Ƃ������\n";
            cout << "������:���O����ɁC���@�̎g�����������Ă������C�Z�~�i�[�C���Ȃ珉����芄���C30000G�łǂ������H\n";
            cout << "���l�ǂ������݂�����,,,\n";
            cout << "1.�w������\n" << "2.�w�����Ȃ�\n";
            int selectbuy;
            cin >> selectbuy;
            if (selectbuy == 1) {
                if (player.getMoney() >= 30000) {
                    player.spendMoney(30000);
                    cout << "30000G�������D\n";
                    Weapon newWeapon("�����g�R�̏�", 75);
                    Armor newArmor("�����̃��[�u�i�j�p�j", 30);
                    player.equipWeapon(newWeapon);
                    player.equipArmor(newArmor);
                }
                else if (player.getMoney() < 30000) {
                    cout << "����������Ȃ������c\n";
                }
            }
            else if (selectbuy == 2) {
                cout << "�����̂́C��߂Ă������c�i���l�ǂɂ͂������ȁj\n";
                cout << "��������U�􂵂Ă݂�!\n";
                displayhomunculus();
                cout << "�����̗��ɂ���,�����̃z�����N���X�Ɛ����\n";
                Enemy enemy("�z�����N���X", 135, 10);
                player.battle(enemy);
                if (!player.stillAlive()) {
                    player.Storyend();
                    return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
                }
            }
            cout << "���l��:���������́C����ɋ��������X�^�[��������!�撣���Ă�����!\n";
            cout << "�����̖���������\n";
            HealingItem witchmedicine("�����̖�", 99);
            player.addhealingitem(witchmedicine);
        }
        else if (choice == 3) {
            cout << "�z�R�̒��ɂ́C�F�X�ȍz�΂���������\n";
            cout << "�����@���āC�����������!\n";
            Weapon newWeapon("����Sword", 70);
            player.equipWeapon(newWeapon);
            displaygolem();
            cout << "�~�����Ė����Ō@���Ă�����,�S�[�����ɏP��ꂽ\n";
            Enemy enemy("�S�[����", 500, 45);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�S�[�����𗐊l���Ĕ�������C���������ɂȂ肻����!\n";
            cout << "GET��������S�[�����𔄂��āC�񕜖���������܍w������\n";
            HealingItem megapotion("���K�|�[�V����", 150);
            player.addhealingitem(megapotion);
            HealingItem medicine("����", 80);
            player.addhealingitem(medicine);
        }
        break;
    case 4:
        if (choice == 1) {
            if (player.getInventory().empty()) {
                cout << "�����̏闈���Ƃ����̂ɁC���͉��̕�����h��������Ă��Ȃ�....\n";
                cout << "�M��������̂́C�Ȃ̌��݂̂��u���v��!\n";
                cout << player.getName() << "�͊o�����C�����ƂɃN���X�`�F���W����!(���Ă܂���j\n";
                Weapon newWeapon("�Ȃ̌�", 9999);//����������Ȃ����[�g(1���[�g�̂݁j��I�������ꍇ�C�����ōŋ������GET
                player.equipWeapon(newWeapon);
            }
            displayCerberus();
            cout << "�����̏�ɓ�����...������͔��Â�,�ǂ�`���ē��Ȃ�ɐi��ł���\n";
            cout << "�����̕󕨌ɂ�������!�C���F���Ă���ƁC�P���x���X���P���|�����Ă���\n";
            Enemy enemy1("�P���x���X", 999, 45);
            player.battle(enemy1);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�󕨌ɂ���C�A�C�e������ɓ��ꂽ\n";
            Weapon newWeapon("�����̌�", 150);
            player.equipWeapon(newWeapon);
            Armor newArmor("���g�̊Z", 75);
            player.equipArmor(newArmor);
            HealingItem HyperPotion("�n�C�p�[�|�[�V����", 350);
            player.addhealingitem(HyperPotion);
            player.usehealingItem();
            displaymaou();
            cout << "����(Hard)�����ꂽ�CHard�Ȃ̂ŋ�������\n";
            cout << "�悭������" << player.getName() << "��C�����ł����΂邪����!!\n";
            Enemy enemy2("����(Hard)", 1230, 115);
            player.battle(enemy2);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }


        }
        else if (choice == 2) {
            cout << "�R�����銈�ΎR�ɂ����C�ܔM�̋�C��������ɂ������߂Ă���\n";
            cout << "�Ό��t�߂ɁC��������ȃ����X�^�[��������!\n";
            displayifrit();
            cout << "�ܔM�̖��_�C�C�t���[�g�Ɩڂ������Ă��܂���\n";
            Enemy enemy("�C�t���[�g", 500, 84);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "�U�����ƂĂ��ɂ��C�育�킢���肾����\n";
            cout << "�Ό��̃C�t���[�g�������ꏊ�ɁC����������!\n";
            Weapon newWeapon("�����C�t���[�g", 175);
            player.equipWeapon(newWeapon);
        }
        else if (choice == 3) {
            cout << "������ⴂɏ���āC�V��̕��V���Ɍ�������\n";
            displayTempestdragon();
            cout << "�͂邩���ɂ��т����_�a�ɂ́C����ȃh���S�����Ȃ�ł���!\n";
            Enemy enemy("�e���y�X�g�h���S��", 1050, 51);
            player.battle(enemy);
            if (!player.stillAlive()) {
                player.Storyend();
                return;//�v���C���[�����񂾂Ƃ��͂����ŏI��
            }
            cout << "���V���ɂ́C��̖�������!���E�͍L��!\n";
            cout << "�u�h���S���X���C���[�v�̏̍�����ɓ��ꂽ!\n";
        }
        break;

    case 5:
        if (choice == 1) {
            cout << player.getName() << "�́C�����̋ʍ��ɍ���C���E�����Ɍ����ē����n�߂�!\n";
            cout << "�l�̖���!!!�I���˃F!!!\n";
        }
        else if (choice == 2) {
            cout << "Youtuber�Ƃ��āC�����͂���!!\n";
            cout << "5000000G ��ɓ��ꂽ!!\n";
            player.earnMoney(5000000);


        }
        else if (choice == 3) {
            cout << "���̐킢�́C�܂��܂��I���Ȃ�!!\n";
            cout << "(Hard���[�h�ł́C���郋�[�g��ʂ�ƁC�閧�̍ŋ����킪GET�ł���悤!�T���Ă݂Ă�!)\n";
        }
        break;

    }

    advanceHardStory(player, depth + 1); //���̃X�e�[�W�֐i��

}

int main() {
    srand(time(0));
    cout << "�V�т���:�\�����ꂽ�I�����̒��ŁC�ԍ���I��ŁC�X�g�[���[��i�߂čs���Ă�!\n";
    cout << "�G�L�����̉摜���\�����ꂽ���́C�~�{�^���C�Ⴕ���̓L�[�{�[�h�������΁C�X�g�[���[���i�ނ�\n";
    //�v���C���[�̖��O�����߂Ă��炤
    string Heroname;
    cout << "�E�҂̖��O���L�[�{�[�h�œ��͂��ĂˁI: ";
    cin >> Heroname;
    //�v���C���[���쐬�i���O�����[�U�[���͂ɕύX�j
    Hero hero(Heroname, 150, 50, Weapon("�f��", 3), Armor("����", 1));
    cout << "�������炠�Ȃ��̖`�����n�܂�܂��D��Փx��I��Ŕԍ����L�[�{�[�h�œ��͂��Ă�\n" << "1.Normal\n" << "2.Hard\n";
    int select;
    bool validselection = false;
    while (!validselection) {//�Ԉ�������͂͂�����x�I�����񎦂����
        cin >> select;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(100, '\n');//�����ȊO�̓��͂𖳎����čēx�I������
            cout << "�����ȑI��������!1�`2�̊Ԃōē��͂��Ă�\n";
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
                cout << "�����ȑI��������!1�`2�̊Ԃōē��͂��Ă�\n";//1�`2�ȊO�̐����̓��͂�,�ēx�I������
            }
        }
    }
}

