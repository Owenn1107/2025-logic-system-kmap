//testing program
#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <map>

using namespace std;

struct KmapSolution {
    int numVar;
    char **kmap;
    vector<string> primes;
    vector<string> essentials;
    vector<string> minimumSop;
};

struct KmapResult_int {
    vector<vector<int>> primes_int;
    vector<vector<int>> essentials_int;
    vector<vector<int>> minimumSop_int;
};

int rows, cols;
int greycode[4][4];
int edgecase=0;

map<int, vector<int>> greycode_2 = {
    {0, {0, 0}}, {2, {0, 1}},
    {1, {1, 0}}, {3, {1, 1}}
};

map<int, vector<int>> greycode_3 = {
    {0, {0, 0}}, {2, {0, 1}}, {6, {0, 2}}, {4, {0, 3}},
    {1, {1, 0}}, {3, {1, 1}}, {7, {1, 2}}, {5, {1, 3}}
};

map<int, vector<int>> greycode_4 = {
    {0, {0, 0}}, {4, {0, 1}}, {12, {0, 2}}, {8, {0, 3}},
    {1, {1, 0}}, {5, {1, 1}}, {13, {1, 2}}, {9, {1, 3}},
    {3, {2, 0}}, {7, {2, 1}}, {15, {2, 2}}, {11, {2, 3}},
    {2, {3, 0}}, {6, {3, 1}}, {14, {3, 2}}, {10, {3, 3}}
};
// Function to combine minterms into a minimized Boolean expression
string combineMintermsToExpr(int numVar, const vector<int>& group) {
    if (group.empty()) return "";
    
    // Helper function to convert an integer to its binary representation
    auto toBinary = [](int num, int bits) -> string {
        string result = "";
        for (int i = 0; i < bits; i++) {
            result = ((num & 1) ? "1" : "0") + result;
            num >>= 1;
        }
        return result;
    };
    
    // Special handling for numVar = 2
    if (numVar == 2) {
        // Get the appropriate Gray code mapping
        map<int, vector<int>> greycode = greycode_2;
        
        // Convert minterms to their Gray code positions
        vector<vector<int>> positions;
        for (int minterm : group) {
            if (greycode.find(minterm) != greycode.end()) {
                positions.push_back(greycode[minterm]);
            }
        }
        
        if (positions.empty()) return "";
        
        // Track which variables have the same value across all minterms
        vector<bool> sameValue(2, true); // Assume all bits are the same initially
        vector<bool> valueIs1(2, false); // Track if the common value is 1
        
        // For the first position, record its bit values
        for (int i = 0; i < positions[0].size(); i++) {
            valueIs1[i] = (positions[0][i] == 1);
        }
        
        // Check if bits remain the same across all positions
        for (size_t i = 1; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                if ((positions[i][j] == 1) != valueIs1[j]) {
                    sameValue[j] = false; // This bit varies
                }
            }
        }
        
        // Generate the Boolean expression
        string result = "";
        string vars = "ba"; // For numVar = 2, a is MSB, b is LSB
        
        for (int i = 0; i < 2; i++) {
            if (sameValue[i]) {
                // Add the variable to the expression
                result += vars[i];
                
                // Add negation if the common value is 0
                if (!valueIs1[i]) {
                    result += '\'';
                }
            }
        }
        
        return result;
    }
    
    // For numVar = 3 and numVar = 4, directly compare binary representations
    vector<string> binaries;
    for (int minterm : group) {
        binaries.push_back(toBinary(minterm, numVar));
    }
    
    if (binaries.empty()) return "";
    
    // Check which bits are the same across all minterms
    vector<bool> sameValue(numVar, true);
    vector<bool> valueIs1(numVar, false);
    
    // Set initial values from the first minterm
    for (int i = 0; i < numVar; i++) {
        valueIs1[i] = (binaries[0][i] == '1');
    }
    
    // Check if bits remain the same across all minterms
    for (size_t i = 1; i < binaries.size(); i++) {
        for (int j = 0; j < numVar; j++) {
            if ((binaries[i][j] == '1') != valueIs1[j]) {
                sameValue[j] = false; // This bit varies
            }
        }
    }
    
    // Generate the Boolean expression
    string result = "";
    string vars = (numVar == 3) ? "abc" : "abcd";
    
    for (int i = 0; i < numVar; i++) {
        if (sameValue[i]) {
            // Add the variable to the expression
            result += vars[i];
            
            // Add negation if the common value is 0
            if (!valueIs1[i]) {
                result += '\'';
            }
        }
    }
    
    return result;
}


char** allocatekmap(int numVar) {
    switch (numVar) {
        case 2:
            rows = 2;
            cols = 2;
            greycode[0][0] = 0;
            greycode[0][1] = 2;
            greycode[1][0] = 1;
            greycode[1][1] = 3;
            break;
        case 3:
            rows = 2;
            cols = 4;
            greycode[0][0] = 0;
            greycode[0][1] = 2;
            greycode[0][2] = 6;
            greycode[0][3] = 4;
            greycode[1][0] = 1;
            greycode[1][1] = 3;
            greycode[1][2] = 7;
            greycode[1][3] = 5;
            break;
        case 4:
            rows = 4;
            cols = 4;
            greycode[0][0] = 0;
            greycode[0][1] = 4;
            greycode[0][2] = 12;
            greycode[0][3] = 8;
            greycode[1][0] = 1;
            greycode[1][1] = 5;
            greycode[1][2] = 13;
            greycode[1][3] = 9;
            greycode[2][0] = 3;
            greycode[2][1] = 7;
            greycode[2][2] = 15;
            greycode[2][3] = 11;
            greycode[3][0] = 2;
            greycode[3][1] = 6;
            greycode[3][2] = 14;
            greycode[3][3] = 10;
            break;
    }

    // Allocate memory for kmap
    char** kmap = new char*[rows];
    for (int i = 0; i < rows; i++) {
        kmap[i] = new char[cols];
    }

    // Initialize kmap with '0'
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            kmap[i][j] = '0';
        }
    }

    return kmap;
}

void fillkmap(char** kmap, int numVar, const vector<int> minterms, const vector<int> dontcares) {
    //fill in minterms
    for(int i=0; i<minterms.size(); i++){
        for(int j=0; j<rows; j++){
            for(int k=0; k<cols; k++){
                if(minterms[i]==greycode[j][k]){
                    kmap[j][k]='1';
                }
            }
        }
    }
    //fill in dontcares
    for(int i=0; i<dontcares.size(); i++){
        for(int j=0; j<rows; j++){
            for(int k=0; k<cols; k++){
                if(dontcares[i]==greycode[j][k]){
                    kmap[j][k]='x';
                }
            }
        }
    }
    
}

bool checkprime(const vector<int>& candidate, const vector<vector<int>>& primes) {
    // 1. Check if the candidate is equal to any existing prime
    for (const auto& prime : primes) {
        // If the candidate is identical to an existing prime, return false
        if (candidate == prime) {
            return false;
        }
    }
    
    // 2. Check if the candidate is a superset of any existing prime
    for (const auto& prime : primes) {
        // If candidate contains all elements in prime, prime is a subset
        if (prime.size() <= candidate.size()) {
            bool isSuperset = true;
            for (int val : prime) {
                if (find(candidate.begin(), candidate.end(), val) == candidate.end()) {
                    isSuperset = false;
                    break;
                }
            }
            if (isSuperset) {
                return false; // Candidate is a superset of an existing prime
            }
        }
    }
    
    // 3. Check if the candidate is a subset of any existing prime
    for (const auto& prime : primes) {
        // If prime contains all elements in candidate, candidate is a subset
        if (candidate.size() <= prime.size()) {
            bool isSubset = true;
            for (int val : candidate) {
                if (find(prime.begin(), prime.end(), val) == prime.end()) {
                    isSubset = false;
                    break;
                }
            }
            if (isSubset) {
                return false; // Candidate is a subset of an existing prime
            }
        }
    }
    
    return true; // Should add to primes
}
bool checkprime_include(const vector<int>& candidate, const vector<vector<int>>& primes) {
    // No longer check for equality - we want to include duplicates
    
    // Check if the candidate is a subset of any existing prime
    for (const auto& prime : primes) {
        // If prime contains all elements in candidate, candidate is a subset
        if (candidate.size() < prime.size()) {  // Note: changed <= to < to allow equal sets
            bool isSubset = true;
            for (int val : candidate) {
                if (find(prime.begin(), prime.end(), val) == prime.end()) {
                    isSubset = false;
                    break;
                }
            }
            if (isSubset) {
                return false; // Candidate is a proper subset of an existing prime
            }
        }
    }
    
    // Check if the candidate is a superset of any existing prime
    for (const auto& prime : primes) {
        // If candidate contains all elements in prime, prime is a subset
        if (prime.size() < candidate.size()) {  // Note: changed <= to < to allow equal sets
            bool isSuperset = true;
            for (int val : prime) {
                if (find(candidate.begin(), candidate.end(), val) == candidate.end()) {
                    isSuperset = false;
                    break;
                }
            }
            if (isSuperset) {
                return false; // Candidate is a proper superset of an existing prime
            }
        }
    }
    
    return true; // Should add to primes
}
vector<vector<int>> deletesame_essentials(const vector<vector<int>>& essentials) {
    vector<vector<int>> result;
    
    for (const auto& current : essentials) {
        bool isDuplicate = false;
        
        // Copy and sort the current implicant for comparison
        vector<int> sortedCurrent = current;
        sort(sortedCurrent.begin(), sortedCurrent.end());
        
        // Check if this implicant is already in the result (in any order)
        for (const auto& existing : result) {
            if (sortedCurrent.size() != existing.size()) continue;
            
            // Make a sorted copy of the existing implicant
            vector<int> sortedExisting = existing;
            sort(sortedExisting.begin(), sortedExisting.end());
            
            // Compare the sorted vectors
            if (sortedCurrent == sortedExisting) {
                isDuplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!isDuplicate) {
            result.push_back(current);
        }
    }
    
    return result;
}



vector<vector<int>> findMinimumSOP(const vector<vector<int>>& primes, const vector<vector<int>>& essentials, const vector<int>& minterms) {
    vector<vector<int>> result;
    unordered_set<int> covered;
    unordered_set<int> allMinterms(minterms.begin(), minterms.end());

    // Step 1: Include all essential implicants directly
    for (const auto& essential : essentials) {
        // Add each essential implicant to the result
        bool isAlreadyIncluded = false;
        
        // Check if this essential is already included in result
        for (const auto& r : result) {
            if (r == essential) {
                isAlreadyIncluded = true;
                break;
            }
        }
        
        if (!isAlreadyIncluded) {
            result.push_back(essential);
            
            // Mark minterms as covered
            for (int x : essential) {
                if (allMinterms.count(x)) {
                    covered.insert(x);
                }
            }
        }
    }

    // Step 2: Greedy algorithm to cover remaining minterms
    while (covered.size() < allMinterms.size()) {
        int bestIndex = -1;
        int bestCover = 0;

        for (int i = 0; i < primes.size(); ++i) {
            // Skip if this prime is already in result
            bool alreadyIncluded = false;
            for (const auto& r : result) {
                if (r == primes[i]) {
                    alreadyIncluded = true;
                    break;
                }
            }
            if (alreadyIncluded) continue;

            // Count how many uncovered minterms this prime would cover
            int count = 0;
            for (int m : primes[i]) {
                if (allMinterms.count(m) && !covered.count(m))
                    count++;
            }

            if (count > bestCover) {
                bestCover = count;
                bestIndex = i;
            }
        }

        if (bestIndex == -1)
            break;

        result.push_back(primes[bestIndex]);
        for (int x : primes[bestIndex]) {
            if (allMinterms.count(x)) {
                covered.insert(x);
            }
        }
    }

    return result;
}

//numVar = 2 case
KmapResult_int kmap_varNum2(char** kmap, KmapSolution* sol, const vector<int>& minterms){
    edgecase = 0;
    vector<vector<int>> essentials_2;
    vector<vector<int>> primes_2;
    vector<vector<int>> minimumSop_2;

    //edge case, all the variables in kmap are 'l' or 'x'
    if ((kmap[0][0] == '1' || kmap[0][0] == 'x') && 
        (kmap[0][1] == '1' || kmap[0][1] == 'x') && 
        (kmap[1][0] == '1' || kmap[1][0] == 'x') && 
        (kmap[1][1] == '1' || kmap[1][1] == 'x')) {
        edgecase = 1; // all 1
        KmapResult_int result;
        result.primes_int = {{0, 1, 2, 3}};
        result.essentials_int = {{0, 1, 2, 3}};
        result.minimumSop_int = {{0, 1, 2, 3}};
        return result;
    }
    //edge case, all 0
    if ((kmap[0][0] == '0') && 
        (kmap[0][1] == '0') && 
        (kmap[1][0] == '0') && 
        (kmap[1][1] == '0')) {
        edgecase = 2; // all 0
        KmapResult_int result;
        result.primes_int = {{}};
        result.essentials_int = {{}};
        result.minimumSop_int = {{}};
        return result;
    }

    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            int used=0;
            int essential_used=0;
             
            if(kmap[i][j]=='1'){
                //circle 1x2 cases
                if(kmap[i][(j+1)%2]=='1' || kmap[i][(j+1)%2]=='x'){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%2]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%2]};
                    if(checkprime_include(candidate, essentials_2)){
                        essentials_2.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_2)){
                        primes_2.push_back(candidate);
                    }
                    
                }
                //circle 2x1 cases
                if(kmap[(i+1)%2][j]=='1' || kmap[(i+1)%2][j]=='x'){
                    //push back to essentials with greycode[i][j] and greycode[(i+1)%2][j]
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%2][j]};
                    
                    if(checkprime_include(candidate, essentials_2)){
                        essentials_2.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                  
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_2)){
                        primes_2.push_back(candidate);
                    }
                }
                //deal with essentials and single 1x1 case
                if (essential_used > 1 ) {
                    //remove the last 'used' essentials
                    for(int k = 0; k < essential_used; k++) {
                        essentials_2.pop_back();
                    }
                }
                else if (used == 0) {
                    vector<int> candidate = {greycode[i][j]};
                    essentials_2.push_back(candidate);
                    primes_2.push_back(candidate);
                }

            }
        }
    }
    essentials_2=deletesame_essentials(essentials_2);
    minimumSop_2 = findMinimumSOP(primes_2, essentials_2, minterms);

     // Create and populate the result
     KmapResult_int result;
     result.primes_int = primes_2;
     result.essentials_int = essentials_2;
     result.minimumSop_int = minimumSop_2;
     
     return result;
};

//numVar = 3 case
KmapResult_int kmap_varNum3(char** kmap, KmapSolution* sol, const vector<int>& minterms){
    edgecase = 0;
    vector<vector<int>> essentials_3;
    vector<vector<int>> primes_3;
    vector<vector<int>> minimumSop_3;
    KmapResult_int result;
    //edge case, all the variables in kmap are 'l' or 'x'
    if ((kmap[0][0] == '1' || kmap[0][0] == 'x') && 
        (kmap[0][1] == '1' || kmap[0][1] == 'x') && 
        (kmap[0][2] == '1' || kmap[0][2] == 'x') && 
        (kmap[0][3] == '1' || kmap[0][3] == 'x') && 
        (kmap[1][0] == '1' || kmap[1][0] == 'x') && 
        (kmap[1][1] == '1' || kmap[1][1] == 'x') && 
        (kmap[1][2] == '1' || kmap[1][2] == 'x') && 
        (kmap[1][3] == '1' || kmap[1][3] == 'x')) {
        edgecase = 1; // all 1
        KmapResult_int result;
        result.primes_int = {{0, 1, 2, 3, 4, 5, 6, 7}};
        result.essentials_int = {{0, 1, 2, 3, 4, 5, 6, 7}};
        result.minimumSop_int = {{0, 1, 2, 3, 4, 5, 6, 7}};
        return result;
    }
    //edge case, all 0
    if ((kmap[0][0] == '0') && 
        (kmap[0][1] == '0') && 
        (kmap[0][2] == '0') && 
        (kmap[0][3] == '0') && 
        (kmap[1][0] == '0') && 
        (kmap[1][1] == '0') && 
        (kmap[1][2] == '0') && 
        (kmap[1][3] == '0')) {
        edgecase = 2; // all 0
        KmapResult_int result;
        result.primes_int = {{}};
        result.essentials_int = {{}};
        result.minimumSop_int = {{}};
        return result;
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            int essential_used=0;
            int used=0;
            if(kmap[i][j]=='1'){
                //2x2_right cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[(i+1)%2][j]=='1' || kmap[(i+1)%2][j]=='x') &&
                   (kmap[(i+1)%2][(j+1)%4]=='1' || kmap[(i+1)%2][(j+1)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[(i+1)%2][j], greycode[(i+1)%2][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                //2x2_left cases
                if((kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x') &&
                   (kmap[(i+1)%2][j]=='1' || kmap[(i+1)%2][j]=='x') &&
                   (kmap[(i+1)%2][(j+3)%4]=='1' || kmap[(i+1)%2][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+3)%4], greycode[(i+1)%2][j], greycode[(i+1)%2][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                //1x4 cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[i][(j+2)%4]=='1' || kmap[i][(j+2)%4]=='x') &&
                   (kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[i][(j+2)%4], greycode[i][(j+3)%4]};

                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                //1x2_right cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                        
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                //1x2_left cases
                if((kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                //2x1 cases
                if((kmap[(i+1)%2][j]=='1' || kmap[(i+1)%2][j]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[(i+1)%2][j]
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%2][j]};               
                    if(checkprime_include(candidate, essentials_3)){
                        essentials_3.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_3)){
                        primes_3.push_back(candidate);
                    }
                }
                if (essential_used > 1 ) {
                    //remove the last 'used' essentials
                    for(int k = 0; k < essential_used; k++) {
                        essentials_3.pop_back();
                    }
                }
                else if (used == 0) {
                    vector<int> candidate = {greycode[i][j]};
                    essentials_3.push_back(candidate);
                    primes_3.push_back(candidate);
                }
            }
        }
    }
    essentials_3=deletesame_essentials(essentials_3);
    minimumSop_3 = findMinimumSOP(primes_3, essentials_3, minterms);
    
    result.primes_int = primes_3;   
    result.essentials_int =essentials_3;
    result.minimumSop_int = minimumSop_3;
    
    return result;
}

//numVar = 4 case
KmapResult_int kmap_varNum4(char** kmap, KmapSolution* sol, const vector<int>& minterms){
    edgecase = 0;
    vector<vector<int>> essentials_4;
    vector<vector<int>> primes_4;
    vector<vector<int>> minimumSop_4;
    KmapResult_int result;
    //edge case, all the variables in kmap are 'l' or 'x'
    if ((kmap[0][0] == '1' || kmap[0][0] == 'x') && 
        (kmap[0][1] == '1' || kmap[0][1] == 'x') && 
        (kmap[0][2] == '1' || kmap[0][2] == 'x') && 
        (kmap[0][3] == '1' || kmap[0][3] == 'x') && 
        (kmap[1][0] == '1' || kmap[1][0] == 'x') && 
        (kmap[1][1] == '1' || kmap[1][1] == 'x') && 
        (kmap[1][2] == '1' || kmap[1][2] == 'x') && 
        (kmap[1][3] == '1' || kmap[1][3] == 'x') &&
        (kmap[2][0] == '1' || kmap[2][0] == 'x') && 
        (kmap[2][1] == '1' || kmap[2][1] == 'x') && 
        (kmap[2][2] == '1' || kmap[2][2] == 'x') && 
        (kmap[2][3] == '1' || kmap[2][3] == 'x') &&
        (kmap[3][0] == '1' || kmap[3][0] == 'x') && 
        (kmap[3][1] == '1' || kmap[3][1] == 'x') && 
        (kmap[3][2] == '1' || kmap[3][2] == 'x') && 
        (kmap[3][3] == '1' || kmap[3][3] == 'x')) {
        edgecase = 1;
        // all 1
        KmapResult_int result;
        result.primes_int = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
        result.essentials_int = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
        result.minimumSop_int = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
        return result;
    }
    //edge case, all 0
    if ((kmap[0][0] == '0') && 
        (kmap[0][1] == '0') && 
        (kmap[0][2] == '0') && 
        (kmap[0][3] == '0') && 
        (kmap[1][0] == '0') && 
        (kmap[1][1] == '0') && 
        (kmap[1][2] == '0') && 
        (kmap[1][3] == '0') &&
        (kmap[2][0] == '0') && 
        (kmap[2][1] == '0') && 
        (kmap[2][2] == '0') && 
        (kmap[2][3] == '0') &&
        (kmap[3][0] == '0') && 
        (kmap[3][1] == '0') && 
        (kmap[3][2] == '0') && 
        (kmap[3][3] == '0')) {
        edgecase = 2; // all 0
        KmapResult_int result;
        result.primes_int = {{}};
        result.essentials_int = {{}};
        result.minimumSop_int = {{}};
        return result;
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            int essential_used=0;
            int used=0;
            if(kmap[i][j]=='1'){
                //2x4 down cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[i][(j+2)%4]=='1' || kmap[i][(j+2)%4]=='x') &&
                   (kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x') &&
                   (kmap[(i+1)%4][j]=='1' || kmap[(i+1)%4][j]=='x') &&
                   (kmap[(i+1)%4][(j+1)%4]=='1' || kmap[(i+1)%4][(j+1)%4]=='x') &&
                   (kmap[(i+1)%4][(j+2)%4]=='1' || kmap[(i+1)%4][(j+2)%4]=='x') &&
                   (kmap[(i+1)%4][(j+3)%4]=='1' || kmap[(i+1)%4][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[i][(j+2)%4], greycode[i][(j+3)%4],
                                             greycode[(i+1)%4][j], greycode[(i+1)%4][(j+1)%4], greycode[(i+1)%4][(j+2)%4], greycode[(i+1)%4][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x4 up cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[i][(j+2)%4]=='1' || kmap[i][(j+2)%4]=='x') &&
                   (kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x') &&
                   (kmap[(i+3)%4][j]=='1' || kmap[(i+3)%4][j]=='x') &&
                   (kmap[(i+3)%4][(j+1)%4]=='1' || kmap[(i+3)%4][(j+1)%4]=='x') &&
                   (kmap[(i+3)%4][(j+2)%4]=='1' || kmap[(i+3)%4][(j+2)%4]=='x') &&
                   (kmap[(i+3)%4][(j+3)%4]=='1' || kmap[(i+3)%4][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[i][(j+2)%4], greycode[i][(j+3)%4],
                                             greycode[(i+3)%4][j], greycode[(i+3)%4][(j+1)%4], greycode[(i+3)%4][(j+2)%4], greycode[(i+3)%4][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //4x2 right cases
                if((kmap[(i+1)%4][j]=='1' || kmap[(i+1)%4][j]=='x') &&
                   (kmap[(i+2)%4][j]=='1' || kmap[(i+2)%4][j]=='x') &&
                    (kmap[(i+3)%4][j]=='1' || kmap[(i+3)%4][j]=='x') &&
                   (kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[(i+1)%4][(j+1)%4]=='1' || kmap[(i+1)%4][(j+1)%4]=='x') &&
                   (kmap[(i+2)%4][(j+1)%4]=='1' || kmap[(i+2)%4][(j+1)%4]=='x') &&
                   (kmap[(i+3)%4][(j+1)%4]=='1' || kmap[(i+3)%4][(j+1)%4]=='x')){
                    
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%4][j],greycode[(i+2)%4][j], greycode[(i+3)%4][j], greycode[i][(j+1)%4],
                                             greycode[(i+1)%4][(j+1)%4], greycode[(i+2)%4][(j+1)%4], greycode[(i+3)%4][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //4x2 left cases
                if((kmap[(i+1)%4][j]=='1' || kmap[(i+1)%4][j]=='x') &&
                   (kmap[(i+2)%4][j]=='1' || kmap[(i+2)%4][j]=='x') &&
                    (kmap[(i+3)%4][j]=='1' || kmap[(i+3)%4][j]=='x') &&
                   (kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x') &&
                   (kmap[(i+1)%4][(j+3)%4]=='1' || kmap[(i+1)%4][(j+3)%4]=='x') &&
                   (kmap[(i+2)%4][(j+3)%4]=='1' || kmap[(i+2)%4][(j+3)%4]=='x') &&
                   (kmap[(i+3)%4][(j+3)%4]=='1' || kmap[(i+3)%4][(j+3)%4]=='x')){
                    
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%4][j],greycode[(i+2)%4][j], greycode[(i+3)%4][j], greycode[i][(j+3)%4],
                                             greycode[(i+1)%4][(j+3)%4], greycode[(i+2)%4][(j+3)%4], greycode[(i+3)%4][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x2 down_right cases
                if((kmap[i][(j+1)%4]=='1'||kmap[i][(j+1)%4]=='x')&&
                    (kmap[(i+1)%4][j]=='1'||kmap[(i+1)%4][j]=='x')&&
                    (kmap[(i+1)%4][(j+1)%4]=='1'||kmap[(i+1)%4][(j+1)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[(i+1)%4][j], greycode[(i+1)%4][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x2 up_right cases
                if((kmap[i][(j+1)%4]=='1'||kmap[i][(j+1)%4]=='x')&&
                    (kmap[(i+3)%4][j]=='1'||kmap[(i+3)%4][j]=='x')&&
                    (kmap[(i+3)%4][(j+1)%4]=='1'||kmap[(i+3)%4][(j+1)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[(i+3)%4][j], greycode[(i+3)%4][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x2 down_left cases
                if((kmap[i][(j+3)%4]=='1'||kmap[i][(j+3)%4]=='x')&&
                    (kmap[(i+1)%4][j]=='1'||kmap[(i+1)%4][j]=='x')&&
                    (kmap[(i+1)%4][(j+3)%4]=='1'||kmap[(i+1)%4][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+3)%4], greycode[(i+1)%4][j], greycode[(i+1)%4][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x2 up_left cases
                if((kmap[i][(j+3)%4]=='1'||kmap[i][(j+3)%4]=='x')&&
                    (kmap[(i+3)%4][j]=='1'||kmap[(i+3)%4][j]=='x')&&
                    (kmap[(i+3)%4][(j+3)%4]=='1'||kmap[(i+3)%4][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+3)%4], greycode[(i+3)%4][j], greycode[(i+3)%4][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //1x4 cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x') &&
                   (kmap[i][(j+2)%4]=='1' || kmap[i][(j+2)%4]=='x') &&
                   (kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4], greycode[i][(j+2)%4], greycode[i][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //4x1 cases
                if((kmap[(i+1)%4][j]=='1' || kmap[(i+1)%4][j]=='x') &&
                   (kmap[(i+2)%4][j]=='1' || kmap[(i+2)%4][j]=='x') &&
                   (kmap[(i+3)%4][j]=='1' || kmap[(i+3)%4][j]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%4][j], greycode[(i+2)%4][j], greycode[(i+3)%4][j]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //1x2_right cases
                if((kmap[i][(j+1)%4]=='1' || kmap[i][(j+1)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                        
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //1x2_left cases
                if((kmap[i][(j+3)%4]=='1' || kmap[i][(j+3)%4]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+3)%4]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+3)%4]};
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x1 down cases
                if((kmap[(i+1)%4][j]=='1' || kmap[(i+1)%4][j]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[(i+1)%2][j]
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%4][j]};               
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                //2x1 up cases
                if((kmap[(i+3)%4][j]=='1' || kmap[(i+3)%4][j]=='x')){
                    //push back to essentials with greycode[i][j] and greycode[(i+1)%2][j]
                    vector<int> candidate = {greycode[i][j], greycode[(i+3)%4][j]};               
                    if(checkprime_include(candidate, essentials_4)){
                        essentials_4.push_back(candidate);
                        essential_used++;
                    }
                    used++;
                    
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_4)){
                        primes_4.push_back(candidate);
                    }
                }
                if (essential_used > 1 ) {
                    //remove the last 'used' essentials
                    for(int k = 0; k < essential_used; k++) {
                        essentials_4.pop_back();
                    }
                }
                else if (used == 0) {
                    vector<int> candidate = {greycode[i][j]};
                    essentials_4.push_back(candidate);
                    primes_4.push_back(candidate);
                }
            }
        }
    }
    essentials_4=deletesame_essentials(essentials_4);
    minimumSop_4 = findMinimumSOP(primes_4, essentials_4, minterms);
    
    result.primes_int = primes_4;   
    result.essentials_int =essentials_4;
    result.minimumSop_int = minimumSop_4;
    return result;    
}

KmapSolution* solveKmap(int numVar, const vector<int> minterms, const vector<int> dontcares) {
    // 1. 初始化 solution
    KmapSolution* sol = new KmapSolution;
    sol->numVar = numVar;
    // 2. 建立並填入 kmap 二維陣列
    
    sol->kmap = allocatekmap(numVar);
    fillkmap(sol->kmap, numVar, minterms, dontcares);
   
    KmapResult_int result_int;
 

    switch(numVar) {
        case 2:
            result_int = kmap_varNum2(sol->kmap, sol, minterms);
            //trun all the primes, essentials, minimumSop to string
            if(edgecase==1){
                sol->primes.push_back("1");
                sol->essentials.push_back("1");
                sol->minimumSop.push_back("1");
            }
            else if (edgecase==2){
                sol->primes.push_back("");
                sol->essentials.push_back("");
                sol->minimumSop.push_back("0");

            }

            else{
                for (const auto& prime : result_int.primes_int) {
                    string expr = combineMintermsToExpr(numVar, prime);
                    sol->primes.push_back(expr);
                }
                for (const auto& essential : result_int.essentials_int) {
                    string expr = combineMintermsToExpr(numVar, essential);
                    sol->essentials.push_back(expr);
                }
                for (const auto& minSop : result_int.minimumSop_int) {
                    string expr = combineMintermsToExpr(numVar, minSop);
                    sol->minimumSop.push_back(expr);
                }

            };

            break;
        case 3:
            result_int = kmap_varNum3(sol->kmap, sol, minterms);
            //trun all the primes, essentials, minimumSop to string
            if(edgecase==1){
                sol->primes.push_back("1");
                sol->essentials.push_back("1");
                sol->minimumSop.push_back("1");
            }
            else if (edgecase==2){
                sol->primes.push_back("");
                sol->essentials.push_back("");
                sol->minimumSop.push_back("0");

            }
            else{
                for (const auto& prime : result_int.primes_int) {
                    string expr = combineMintermsToExpr(numVar, prime);
                    sol->primes.push_back(expr);
                }
                for (const auto& essential : result_int.essentials_int) {
                    string expr = combineMintermsToExpr(numVar, essential);
                    sol->essentials.push_back(expr);
                }
                for (const auto& minSop : result_int.minimumSop_int) {
                    string expr = combineMintermsToExpr(numVar, minSop);
                    sol->minimumSop.push_back(expr);
                }

            };
           
            break;
        case 4:
            result_int = kmap_varNum4(sol->kmap, sol, minterms);
            //trun all the primes, essentials, minimumSop to string
            if(edgecase==1){
                sol->primes.push_back("1");
                sol->essentials.push_back("1");
                sol->minimumSop.push_back("1");
            }
            else if (edgecase==2){
                sol->primes.push_back("");
                sol->essentials.push_back("");
                sol->minimumSop.push_back("0");

            }
            else{
                for (const auto& prime : result_int.primes_int) {
                    string expr = combineMintermsToExpr(numVar, prime);
                    sol->primes.push_back(expr);
                }
                for (const auto& essential : result_int.essentials_int) {
                    string expr = combineMintermsToExpr(numVar, essential);
                    sol->essentials.push_back(expr);
                }
                for (const auto& minSop : result_int.minimumSop_int) {
                    string expr = combineMintermsToExpr(numVar, minSop);
                    sol->minimumSop.push_back(expr);
                }

            };
            break;
    }
    return sol;
}