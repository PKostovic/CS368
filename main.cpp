#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Function Declarations
// Most of the functionality of the character builder will be carried out in 
// supplementary functions listed here. The main method will mostly be 
// collecting and amalgamating data, and controlling the flow of the builder.
static void introduction();

// print functions
static void printSkills(std::vector<string> labels, std::vector<int> values);
static void printAbilityScoresAndMods(std::vector<string> labels, std::vector<int> scores, std::vector<int> mods);
static void printSavingThrows(std::vector<string> labels, std::vector<int> values);
static void printBAB(int bab);
static void printACs(int base, int touch, int flat);

// racially-recommended
static string queryHeight(string race);
static string queryWeight(string race);
static string queryAge(string race);

// stand-alone functions
static string queryName();
static string queryGender();
static string queryDeity();
static string queryAlignment();
static string queryRace();
static string queryClass();

// based off class
static std::vector<int> setAbilityScores(string chosenClass, string race);
static std::vector<int> setSkills(string chosenClass, std::vector<int> abilityMods);
static std::vector<int> setSavingThrows(string chosenClass, std::vector<int> abilityMods);
static int setBaseAttackBonus(string chosenClass);

// based off bab
static int getCombatManeuverBonus(int bab, string size, std::vector<int> abilityMods);
static int getCombatManeuverDefense(int cmb, std::vector<int> abilityMods);

// clerical functions
static std::vector<int> computeAbilityModifiers(std::vector<int> scores);

int main(){

  // known values
  std::vector<string> abilityVec = {"Strength", "Dexterity", "Constitution", "Intelligence", "Wisdom", "Charisma"};
  std::vector<string> skillVec = {"Acrobatics", "Appraise", "Bluff", "Climb", "Craft", "Diplomacy",
				"Disable Device", "Disguise", "Escape Artist", "Fly", "Handle Animal",
				"Heal", "Intimidate", "Knowledge(Arcana)", "Knowledge(Dungeoneering)",
				"Knowledge(Engineering)", "Knowledge(Geography)", "Knowledge(History)",
				"Knowledge(Local)", "Knowledge(Nature)", "Knowledge(Nobility)", 
				"Knowledge(Planes)", "Knowledge(Religion)", "Linguistics", "Perception",
				"Perform", "Profession", "Ride", "Sense Motive", "Sleight of Hand",
				"Spellcraft", "Stealth", "Survival", "Swim", "Use Magic Device"};
  std::vector<string> savingThrowVec = {"Fortitude", "Reflex", "Will"};
  
  // basic information about the character, this has little-to-no impact in-game, 
  // and so will be left up to the user, without safeguards.
  string name, alignment, deity, race, size, gender, age, height, weight;

  // this is the defining feature of every character, most of the further stuff will
  // be based off of this
  string characterClass;

  // speed
  // based off class, race, and armor
  // this is going to be hard coded to 30, since we're basic classes
  int speed = 30;

  // ability scores and their component modifiers.
  // modifiers are calculated as: Math.floor((ABILITY SCORE - 10)/2)
  // there are 
  std::vector<int> abilityScores(6);
  std::vector<int> abilityMods(6);
  std::vector<int> skills(35);

  // initiative is the property that determines who goes first in time-sensitive
  // situations. It's calculated as equal to dex_mod + additional values (some feats
  // and traits increase initiative).
  int initiative;
  
  // Saving throws
  // calculated as a base save + ability mod + magic modifier + misc
  // fort takes CON, ref takes DEX, will takes WIS
  std::vector<int> savingThrows;

  // base attack bonus
  // determined by classes
  int bab;

  // Combat Maneuver
  // CMB = BAB + str_mod + size_mod
  // CMD = BAB + std_mod + dex_mod + size_mod
  int cmb, cmd;

  // list of feats that the player controls
  std::vector<string> feats = {};
  
  // say HI
  introduction();
  cout << endl;

  // start with basic data collection
  name = queryName();
  cout << endl;

  gender = queryGender();
  cout << endl;

  race = queryRace();
  cout << endl;

  deity = queryDeity();
  cout << endl;

  alignment = queryAlignment();
  cout << endl;

  age = queryAge(race);
  cout << endl;

  height = queryHeight(race);
  cout << endl;

  weight = queryWeight(race);
  cout << endl;

  characterClass = queryClass();
  cout << endl;

  // get ability scores for the player.
  abilityScores = setAbilityScores(characterClass, race);
  abilityMods = computeAbilityModifiers(abilityScores);

  // hard coded since all currently offered classes are medium-sized
  size = "Medium";

  // get base attack bonus
  bab = setBaseAttackBonus(characterClass);

  // get combat maneuver stats
  cmb = getCombatManeuverBonus(bab, size, abilityMods);
  cmd = getCombatManeuverDefense(cmd, abilityMods);

  // prompt and handle skill set
  skills = setSkills(characterClass, abilityMods);

  // get saving throws
  savingThrows = setSavingThrows(characterClass, abilityMods);
  
  initiative = abilityMods[1];

  /* 
  printSavingThrows(savingThrowVec, savingThrows);
  printSkills(skillVec, skills);
  */

  cout << "********************************************************************************" << endl;
  cout << endl;
  cout << name << ",\t" << alignment << " " << characterClass << " of " << deity << endl;
  cout << "Age: \t" << age << "\t Gender: \t" << gender << endl;
  cout << "Height: \t" << height << "\tWeight: \t" << weight << endl;
  cout << "Base Attack Bonus: \t" << bab << "\tInitiative: \t" << initiative << endl;
  printSavingThrows(savingThrowVec, savingThrows);
  printAbilityScoresAndMods(abilityVec, abilityScores, abilityMods);
  // print armor class information
  printSkills(skillVec, skills);


} // close main

/*
if(chosenClass == "Barbarian" || chosenClass == "barbarian"){
   
  } else if (chosenClass == "Fighter" || chosenClass == "fighter"){
   
  } else if (chosenClass == "Rogue" || chosenClass == "rogue"){
   
  } else if (chosenClass == "Wizard" || chosenClass == "wizard"){
   
  } else {
    // more to come...
    exit(13);
  }

*/

static void printSkills(std::vector<string> labels, std::vector<int> values){

  cout << "Skills: " << endl;
  for(int i = 0; i < 35; i++){
    cout << "\t" << labels[i] << ": " << values[i] << endl;
  }
}

static void printAbilityScoresAndMods(std::vector<string> labels, std::vector<int> scores, std::vector<int> mods){

  cout << "Ability Scores, Mods: " << endl;
  for(int i = 0; i < 6; i++){
    cout << "\t" << labels[i] << ": " << scores[i] << ", mod = " << mods[i] << endl;
  }

}

static void printSavingThrows(std::vector<string> labels, std::vector<int> values){

  cout << "Saving Throws: " << endl;
  for(int i = 0; i < 3; i++){
    cout << "\t" << labels[i] << ": " << values[i] << endl;
  }

}

static std::vector<int> setSavingThrows(string chosenClass, std::vector<int> abilityMods){

  std::vector<int> returnSavingThrows = {0, 0, 0};
  
  if(chosenClass == "Barbarian" || chosenClass == "barbarian"){
    returnSavingThrows[0] = 2;
  } else if (chosenClass == "Fighter" || chosenClass == "fighter"){
    returnSavingThrows[0] = 2;
  } else if (chosenClass == "Rogue" || chosenClass == "rogue"){
    returnSavingThrows[1] = 2;
  } else if (chosenClass == "Wizard" || chosenClass == "wizard"){
    returnSavingThrows[2] = 2;
  } else {
    // more to come...
    exit(13);
  }

  returnSavingThrows[0] += abilityMods[2];
  returnSavingThrows[1] += abilityMods[1];
  returnSavingThrows[2] += abilityMods[4];

  return returnSavingThrows;
}

static int setBaseAttackBonus(string chosenClass){
  
  int returnBAB = 0;
  
  if(chosenClass == "Barbarian" || chosenClass == "barbarian"){
    returnBAB = 1;
  } else if (chosenClass == "Fighter" || chosenClass == "fighter"){
    returnBAB = 1;
  } else if (chosenClass == "Rogue" || chosenClass == "rogue"){
    returnBAB = 0;
  } else if (chosenClass == "Wizard" || chosenClass == "wizard"){
    returnBAB = 0;
  } else {
    // more to come...
    exit(13);
  }

  return returnBAB;
}

static std::vector<int> setSkills(string chosenClass, std::vector<int> abilityMods){
  
  int numPoints = abilityMods[3];
  std::vector<string> recommendedSkills;
  
  if(chosenClass == "Barbarian" || chosenClass == "barbarian"){
    numPoints += 4;
    recommendedSkills = {"Climb", "Intimidate", "Perception", "Survival", "Swim"}; 
  } else if (chosenClass == "Fighter" || chosenClass == "fighter"){
    numPoints += 2;
    recommendedSkills = {"Climb", "Knowledge(Local)", "Perception"};
  } else if (chosenClass == "Rogue" || chosenClass == "rogue"){
    numPoints += 8;
    recommendedSkills = {"Acrobatics", "Appraise", "Disable Device", "Knowledge(Local)", 
			 "Perception", "Sleight of Hand"};
  } else if (chosenClass == "Wizard" || chosenClass == "wizard"){
    numPoints += 2;
    // coming soon
  } else {
    // more to come...
    exit(13);
  }

  cout << "A large array of skills is available to the aspiring adventurer. Each one may" << endl;
  cout << "one day save your life, but you only have a few skill points to spend at each level." << endl;
  cout << "Below is a list of skills, you have " << numPoints << " skill points to spend." << endl;
  cout << "\t01. Acrobatics" << endl;
  cout << "\t02. Appraise" << endl;
  cout << "\t03. Bluff" << endl;
  cout << "\t04. Climb" << endl;
  cout << "\t05. Craft" << endl;
  cout << "\t06. Diplomacy" << endl;
  cout << "\t07. Disable Device" << endl;
  cout << "\t08. Disguise" << endl;
  cout << "\t09. Escape Artist" << endl;
  cout << "\t10. Fly" << endl;
  cout << "\t11. Handle Animal" << endl;
  cout << "\t12. Heal" << endl;
  cout << "\t13. Intimidate" << endl;
  cout << "\t14. Knowledge(Arcana)" << endl;
  cout << "\t15. Knowledge(Dungeoneering)" << endl;
  cout << "\t16. Knowledge(Engineering)" << endl;
  cout << "\t17. Knowledge(Geography)" << endl;
  cout << "\t18. Knowledge(History)" << endl;
  cout << "\t19. Knowledge(Local)" << endl;
  cout << "\t20. Knowledge(Nature)" << endl;
  cout << "\t21. Knowledge(Nobility)" << endl;
  cout << "\t22. Knowledge(Planes)" << endl;
  cout << "\t23. Knowledge(Religion)" << endl;
  cout << "\t24. Linguistics" << endl;
  cout << "\t25. Perception" << endl;
  cout << "\t26. Perform" << endl;
  cout << "\t27. Profession" << endl;
  cout << "\t28. Ride" << endl;
  cout << "\t29. Sense Motive" << endl;
  cout << "\t30. Sleight of Hand" << endl;
  cout << "\t31. Spellcraft" << endl;
  cout << "\t32. Stealth" << endl;
  cout << "\t33. Survival" << endl;
  cout << "\t34. Swim" << endl;
  cout << "\t35. Use Magic Device" << endl;

  cout << "At level one, you can only put one point into any given skill." << endl;  
  std::vector<int> chosenSkills = {0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0,
				   0, 0, 0, 0, 0, 0, 0};
  
  for(int i = 0; i < numPoints; i++){
    cout << "You have " << (numPoints-i) << " left.";
    cout << "Please enter the number of the skill that you would like to put a point into: ";
    
    string c;
    std::getline(cin, c);
    int num = stoi(c);
    
    if(num > 35 || num < 1){
      cout << "Sorry, invalid entry, please try again..." << endl;
      i--;
    } else {
      chosenSkills[num] += 1;
    }
  
  }
  
  chosenSkills[0] += abilityMods[1];
  chosenSkills[1] += abilityMods[3];
  chosenSkills[2] += abilityMods[5];
  chosenSkills[3] += abilityMods[0];
  chosenSkills[4] += abilityMods[3];
  chosenSkills[5] += abilityMods[5];
  chosenSkills[6] += abilityMods[1];
  chosenSkills[7] += abilityMods[5];
  chosenSkills[8] += abilityMods[1];
  chosenSkills[9] += abilityMods[1];
  chosenSkills[10] += abilityMods[5];
  chosenSkills[11] += abilityMods[4];
  chosenSkills[12] += abilityMods[5];
  chosenSkills[13] += abilityMods[3];
  chosenSkills[14] += abilityMods[3];
  chosenSkills[15] += abilityMods[3];
  chosenSkills[16] += abilityMods[3];
  chosenSkills[17] += abilityMods[3];
  chosenSkills[18] += abilityMods[3];
  chosenSkills[19] += abilityMods[3];
  chosenSkills[20] += abilityMods[3];
  chosenSkills[21] += abilityMods[3];
  chosenSkills[22] += abilityMods[3];
  chosenSkills[23] += abilityMods[3];
  chosenSkills[24] += abilityMods[4];
  chosenSkills[25] += abilityMods[5];
  chosenSkills[26] += abilityMods[4];
  chosenSkills[27] += abilityMods[1];
  chosenSkills[28] += abilityMods[4];
  chosenSkills[29] += abilityMods[1];
  chosenSkills[30] += abilityMods[3];
  chosenSkills[31] += abilityMods[1];
  chosenSkills[32] += abilityMods[4];
  chosenSkills[33] += abilityMods[0];
  chosenSkills[34] += abilityMods[5];
  
  return chosenSkills;
}


static int getCombatManeuverBonus(int bab, string size, std::vector<int> abilityMods){
  
  int sizeMod = 0;

  if(size == "Fine"){
    sizeMod -= 8;
  } else if (size == "Diminutive"){
    sizeMod -= 4;
  } else if (size == "Tiny"){
    sizeMod -= 2;
  } else if (size == "Small"){
    sizeMod -= 1;
  } else if (size == "Medium"){
    sizeMod = 0;
  } else if (size == "Large"){
    sizeMod += 1;
  } else if (size == "Huge"){
    sizeMod += 2;
  } else if (size == "Gargantuan"){
    sizeMod += 4;
  } else if (size == "Colossal"){
    sizeMod += 8;
  }

  return bab + abilityMods[0] + sizeMod;
}

static int getCombatManeuverDefense(int cmb, std::vector<int> abilityMods){

  return 10 + cmb + abilityMods[1];
}


// set recommended ability scores, based off of a simple stat array (15, 14, 14, 14, 12, 7)
static std::vector<int> setAbilityScores(string chosenClass, string race){
  int humanFavoredStat;
  std::vector<int> returnAbilityScores = {0,0,0,0,0,0};
  // (STR, DEX, CON, INT, WIS, CHA)
  
  if(chosenClass == "Barbarian" || chosenClass == "barbarian"){
    returnAbilityScores[0] = 14;
    returnAbilityScores[1] = 14;
    returnAbilityScores[2] = 15;
    returnAbilityScores[3] = 12;
    returnAbilityScores[4] = 14;
    returnAbilityScores[5] = 7;
    humanFavoredStat = 2; // barbarians favor charmisma
  } else if (chosenClass == "Fighter" || chosenClass == "fighter"){
    returnAbilityScores[0] = 15;
    returnAbilityScores[1] = 14;
    returnAbilityScores[2] = 14;
    returnAbilityScores[3] = 12;
    returnAbilityScores[4] = 14;
    returnAbilityScores[5] = 7;
    humanFavoredStat = 0; // fighters favor strength
  } else if (chosenClass == "Rogue" || chosenClass == "rogue"){
    returnAbilityScores[0] = 12;
    returnAbilityScores[1] = 15;
    returnAbilityScores[2] = 14;
    returnAbilityScores[3] = 7;
    returnAbilityScores[4] = 14;
    returnAbilityScores[5] = 14;
    humanFavoredStat = 1; // rogues favor dexterity
  } else if (chosenClass == "Wizard" || chosenClass == "wizard"){
    // coming soon
  } else {
    // more to come...
    exit(13);
  }

  if(race == "elf" || race == "Elf"){
    // elves get +2 to DEX and INT, -2 to CON
    returnAbilityScores[1] = returnAbilityScores[1] + 2;
    returnAbilityScores[2] = returnAbilityScores[2] - 2;
    returnAbilityScores[3] = returnAbilityScores[3] + 2;
  } else if (race == "human" || race == "Human"){
    returnAbilityScores[humanFavoredStat] = returnAbilityScores[humanFavoredStat] + 2;
  } else if (race == "dwarf" || race == "Dwarf"){
    // dwarves +2 to CON and WIS, -2 to CHA
    returnAbilityScores[2] = returnAbilityScores[2] + 2;
    returnAbilityScores[5] = returnAbilityScores[5] - 2;
    returnAbilityScores[4] = returnAbilityScores[4] + 2;
  } else {
    // more to come...
    exit(13);
  }

  /*
  for(int i = 0; i < 6; i++){
    cout << "ability score(" << i << ") = " << returnAbilityScores[i] << endl;
  }  
  */

  return returnAbilityScores;
}

static std::vector<int> computeAbilityModifiers(std::vector<int> scores){
  
  std::vector<int> returnAbilityMods = {0,0,0,0,0,0};
  for(int i = 0; i < 6; i++){
    returnAbilityMods[i] = (scores[i] - 10)/2;
  }
  return returnAbilityMods;
}

static string queryClass(){
  cout << "****************************************************************************************" << endl;
  cout << "So now we've come to the most important part, picking a character class!" << endl;
  cout << "Character classes have different skills, specializations, and options for" << endl;
  cout << "the aspiring adventurer. Check out some of the sweet classes available in Pathfinder" << endl;
  cout << "\tBarbarian: you smash, you bash, and you don't read too well. You're a brawny" << endl;
  cout << "\tbadass with no fear, a heart full of courage, and a desire to yell 'Barbarian RAGE' " << endl;
  cout << "\tat inopportune times." << endl;
  cout << "\tFighter: the jack of all trades, and master of none. A versatile martial warrior, " << endl;
  cout << "\thardened and tough, but strategic and careful. Whether for gold, glory, or gods, one " << endl;
  cout << "\tthing ties all fighters together; at the end of the day, only battle matters." << endl;
  cout << "\tRogue: affectionately termed 'skill-monkey', the rogue is the glue that holds adventure" << endl;
  cout << "\tgroups together. Equipped with a vast array of sneaky, useful skills, the rogue is often" << endl;
  cout << "\tfound deep in enemy territory, often dagger-deep in some beast's back, with a cocky smile" << endl;
  cout << "\tand a heroic new tale to tell the local bar-maids." << endl;
  cout << "\tWizard: a true master of the dark and arcane secrets of the ancients, the wizard is the" << endl;
  cout << "\twalking embodiment of what happens when great arrogance meets greater power. Don't be" << endl;
  cout << "\tsurprised if the party wizard constantly acts like they're smarter than you. Honestly," << endl;
  cout << "\tthey probably are" << endl;
  cout << "****************************************************************************************" << endl;
  cout << "Select your class by inputting the name below:" << endl;

  string c;
  std::getline(cin, c);
  return c;

}

static string queryAge(string race){
  int lower_bound;
  int upper_bound;
  string player_race;
  if(race == "elf" || race == "Elf"){
    lower_bound = 114;
    upper_bound = 170;
    player_race = "elven";
  } else if (race == "human" || race == "Human"){
    lower_bound = 16;
    upper_bound = 27;
    player_race = "human";
  } else if (race == "dwarf" || race == "Dwarf"){
    lower_bound = 43;
    upper_bound = 82;
    player_race = "dwarvish";
  } else {
    // more to come...
    exit(13);
  }
  
  cout <<"How old is your character? The usual age" <<endl;
  cout <<"for " << player_race << " adventurers is between " << lower_bound << endl; 
  cout <<"and " << upper_bound << " years of age. This statistic is cosmetic, " <<endl;
  cout <<"and doesn't have any major in-game consequences." << endl;
  cout <<"Please enter your character's age: " << endl;

  string a;
  std::getline(cin, a);
  return a;
}

static string queryHeight(string race){
  int lower_bound;
  int upper_bound;
  string player_race;
  if(race == "elf" || race == "Elf"){
    lower_bound = 66;
    upper_bound = 80;
    player_race = "elves";
  } else if (race == "human" || race == "Human"){
    lower_bound = 60;
    upper_bound = 78;
    player_race = "humans";
  } else if (race == "dwarf" || race == "Dwarf"){
    lower_bound = 45;
    upper_bound = 53;
    player_race = "dwarves";
  } else {
    // more to come...
    exit(13);
  }

  cout <<"How tall is your character? The usual height" <<endl;
  cout <<"for " << player_race << " is between " << lower_bound << endl; 
  cout <<"and " << upper_bound << " inches. This statistic is cosmetic, " <<endl;
  cout <<"and doesn't have any major in-game consequences." << endl;
  cout <<"Please enter your character's height: " << endl;

  string h;
  std::getline(cin, h);
  return h;
}

static string queryWeight(string race){
  int lower_bound;
  int upper_bound;
  string player_race;
  if(race == "elf" || race == "Elf"){
    lower_bound = 116;
    upper_bound = 158;
    player_race = "elves";
  } else if (race == "human" || race == "Human"){
    lower_bound = 130;
    upper_bound = 220;
    player_race = "humans";
  } else if (race == "dwarf" || race == "Dwarf"){
    lower_bound = 116;
    upper_bound = 158;
    player_race = "dwarves";
  } else {
    // more to come...
    exit(13);
  }

  cout <<"What is your character's weight? The usual weight" <<endl;
  cout <<"for " << player_race << " is between " << lower_bound << endl; 
  cout <<"and " << upper_bound << " pounds. This statistic is cosmetic, " <<endl;
  cout <<"and doesn't have any major in-game consequences." << endl;
  cout <<"Please enter your character's weight:" << endl;
  string w;
  std::getline(cin, w);
  return w;

}

// ask the user to pick a race
static string queryRace(){
  cout << "****************************************************************************************" << endl;
  cout << "There are plenty of races available to play in Pathfinder. For the" <<endl;
  cout << "sake of simplicity, we present a few of the most common races, " <<endl;
  cout << "found in almost every game of Pathfinder and every setting." <<endl;
  cout << "At the end, please selest one of the races listed below and type "<<endl;
  cout << "\tHuman: The most versatile of adventurers, found everywhere in the "<<endl;
  cout << "\tworlds. Humans get a +2 bonus to one of their stats, which this "<<endl;
  cout << "\tgenerator will handle internally." <<endl <<endl;
  cout << "\tElf: tall, noble, and often haughty, elves are the long-lived and "<<endl;
  cout << "\tsilent masters of the natural and mystical world. Possessed of a long "<<endl;
  cout << "\tlifespan, elves tend to be dexterous and intelligent, but frail"<<endl;
  cout << endl;
  cout << "\tDwarf: stoic and stern, dwarves are hardy and tough mountain-dwellers, "<<endl;
  cout << "\texcelling in blacksmithing, craftsmanship, and trade. They live "<<endl;
  cout << "\tlonge lives than humans, but they do not grow as ancient as elves."<<endl;
  cout << "\tYour average dwarf is a hearty, wise fellow, but not very charismatic"<<endl;
  cout << "****************************************************************************************" << endl;
  
  // create a placeholder, and wait until the input is something else...
  string r;
  std::getline(cin, r);
  return r;
}

// ask the user for their Deity
static string queryDeity(){
  cout << "****************************************************************************************" << endl;
  cout << "Pathfinder takes place in a world with many deities." <<endl;
  cout << "A deity is related to what a person does and believes in,"  <<endl;
  cout << "so think about what kind of life your character had, and " <<endl;
  cout << "what they would care about. Choose one that represents your " <<endl;
  cout << "character's life, or enter 'None' if your character doesn't " <<endl;
  cout << "believe in any gods." <<endl;
  cout << "Here are a few examples: " <<endl;
  
  cout << "\tAbadar - god of cities, law, merchants, and wealth" <<endl;
  cout << "\tAsmodeus - god of tyranny, slavery, pride and contracts" <<endl;
  cout << "\tCalistria - goddess of trickery, lust, and revenge" <<endl;
  cout << "\tCayden Cailean - god of freedom, ale, wine, and bravery" <<endl;
  cout << "\tDesna - goddess of drams, starts, luck, and travelers" <<endl;
  cout << "\tErastil - god of family, farming, hunting, and trade" <<endl;
  cout << "\tGorum - god of strength, battle, and weapons" << endl;
  cout << "\tGozreh - goddess of nature, weather, storms, and the sea" <<endl;
  cout << "\tIomedae - goddess of honor, justice, rulership, and valor" <<endl;
  cout << "\tIrori - god of history, knowledge, self-perfection, and health" <<endl;
  cout << "\tLamashtu - goddess of madness, monsters, nightmares, and evil" <<endl;
  cout << "\tNethys - god of magic, knowledge, and runes" <<endl;
  cout << "\tNorgorber - god of greed, secrets, poison, and murder" <<endl;
  cout << "\tPharasma - goddess of fate, death, rebirth, and prophecy" <<endl;
  cout << "\tRovagug - god of wrath, disaster, and destruction"  <<endl;
  cout << "\tSarenrae - goddess of redemption, honesty, healing, and the sun" <<endl;
  cout << "\tShelyn - goddess of beauty, art, love, and music"  <<endl;
  cout << "\tTorag - god of the forge, protection, strategy, and dwarves"  <<endl; 
  cout << "\tUrgothoa - goddess of gluttony, disease, undead, and excess"  <<endl;
  cout << "\tZon-Kuthon - god of envy, pain, darkness, and loss"  <<endl;  
  cout << "****************************************************************************************" << endl;
  // create a placeholder, and wait until the input is something else...
  string d;
  std::getline(cin, d);
  return d;
}


// ask the user for their alignment
static string queryAlignment(){
  cout << "****************************************************************************************" << endl;
  cout << "One of the major parts of a character is their 'alignment'."  <<endl;
  cout << "Alignment is a way of defining how the character approachs the world"<<endl;
  cout << "essentially it is a label of their moral compass." <<endl;
  cout << "We measure alignment on two axes, Law/Chaos and Good/Evil." <<endl;
  cout << "\tThe Law/Chaos axis describes how much your character likes to" <<endl;
  cout << "\tobey codes, these can be anything from laws to personal moral" <<endl;
  cout << "\tcodes. A Lawful person will always try to follow these codes." <<endl; 
  cout << "\tA Chaotic person would see them as stifling, and would reject" <<endl;
  cout << "\t them out of hand."<<endl <<endl;
  cout << "\tThe Good/Evil axis is rather simple, it just describes whether" <<endl;
  cout << "\ta person is more inclined towards the care of others, or" <<endl;
  cout << "\tself-interest."<<endl <<endl;
  cout << "\tEach of these axes also has a Neutral option, for those  who" <<endl;
  cout << "\tdon't lean particularly hard one way or the other."<< endl <<endl;
  
  cout << "Alignment is usually described as a two-letter combination."<<endl;
  cout << "The first represents a person's position on the Law/Chaos axis," <<endl; 
  cout << "with options (L)-Lawful, (N)-Neutral, and (C)-Chaotic." <<endl;
  cout << "Similarly, Good/Evil is represented with the options (G)-Good, ";
  cout << "(N)-Neutral, and (E)-Evil." <<endl <<endl;
  cout << "****************************************************************************************" << endl;
  cout << "Please enter the two-letter combination that describes your character's ";
  cout << "alignment:" << endl;
  

  string a;
  std::getline(cin, a);
  return a;
}



// ask the user for their characters gender
static string queryGender(){
  cout << "What is your character's gender?" << endl;
  string g;
  std::getline(cin, g);
  return g;
}

// ask user for the name of their character
static string queryName(){
  cout << "What would you like to name your character?" << endl;
  string name;
  std::getline(cin, name);
  return name;
} 


// introduce the program to the user
// this could be written more elegantly if we wanted modularity,
// but since its never going to change, we just wrote it to be
// simplistic.
static void introduction(){
  cout << "*****************************************************" << endl;
  cout << "*                                                   *" << endl;
  cout << "*   Welcome to the Pathfinder Character Creator!    *" << endl;
  cout << "*     This app will help you create your first      *" << endl;
  cout << "*   character for the Pathfinder Roleplaying Game   *" << endl;
  cout << "*   Your character will start at level 1 and will   *" << endl;
  cout << "*    have auto-filled information for the details   *" << endl;
  cout << "*                                                   *" << endl;
  cout << "*      Follow the prompts on screen, and your       *" << endl;
  cout << "*   character will be printed at the end. Enjoy!    *" << endl;
  cout << "*                                                   *" << endl;
  cout << "*****************************************************" << endl;
  cout << endl;
} // close introduction()
