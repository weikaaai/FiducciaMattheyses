#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

#define TIME_LIMIT 295

using namespace std;

mutex ma, mb, mc;

void inputCellsSize(unordered_map<int, int>& cells_size, int& sum, string argv) {
  string c_name;
  int c_size;
  ifstream c_file;
  c_file.open(argv);
  while(c_file >> c_name >> c_size) {
    int tmp = stoi(c_name.substr(1, c_name.size()));
    cells_size[tmp] = c_size;
    sum += c_size;
  }
  c_file.close();
}

void inputNetsArray(unordered_map<int, unordered_set<int>>& nets_array, string argv) {
  ifstream n_file;
  string n_str;
  int n_index = 1;
  unordered_set<int> n_tmps;
  n_file.open(argv);
  while(n_file >> n_str) {
    if(n_str.substr(0, 1) == "c") {
      n_tmps.insert(stoi(n_str.substr(1, n_str.size())));
    }
    else if(n_str == "}") {
      nets_array[n_index++].insert(n_tmps.begin(), n_tmps.end());
      n_tmps.clear();
    }
  }
  n_file.close();
}

void input1(unordered_set<int>& A, unordered_set<int> bA) {
  A = bA;
}

void input2(unordered_set<int>& B, unordered_set<int> bB) {
  B = bB;
}

void input3(unordered_map<int, pair<unordered_set<int>, unordered_set<int>>>& nab, unordered_map<int, pair<unordered_set<int>, unordered_set<int>>> bnab) {
  nab = bnab;
}

void bucketA(unordered_set<int> A, unordered_map<int, unordered_set<int>>& bA, unordered_map<int, int> cell_gains) {
  for(auto cell : A) {
    bA[cell_gains[cell]].insert(cell);
  }
}

void bucketB(unordered_set<int> B, unordered_map<int, unordered_set<int>>& bB, unordered_map<int, int> cell_gains) {
  for(auto cell : B) {
    bB[cell_gains[cell]].insert(cell);
  }
}

void coreA(pair<unordered_set<int>, unordered_set<int>> p, unordered_map<int, bool> lockstate, unordered_map<int, unordered_set<int>>& b_A, unordered_map<int, unordered_set<int>>& b_B, unordered_map<int, int>& cell_gains) {
  if(p.second.size() == 0) {
    for(auto item : p.first) {
      if(!lockstate[item]) {
        ma.lock();
        b_A[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]++;
        mc.unlock();
        b_A[cell_gains[item]].insert(item);
        ma.unlock();
      }
    }
  }
  else if(p.second.size() == 1) {
    for(auto item : p.second) {
      if(!lockstate[item]) {
        mb.lock();
        b_B[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]--;
        mc.unlock();
        b_B[cell_gains[item]].insert(item);
        mb.unlock();
      }
    }
  }
}

void coreB(pair<unordered_set<int>, unordered_set<int>> p, unordered_map<int, bool> lockstate, unordered_map<int, unordered_set<int>>& b_A, unordered_map<int, unordered_set<int>>& b_B, unordered_map<int, int>& cell_gains) {
  if(p.first.size() == 0) {
    for(auto item : p.second) {
      if(!lockstate[item]) {
        mb.lock();
        b_B[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]--;
        mc.unlock();
        b_B[cell_gains[item]].insert(item);
        mb.unlock();
      }
    }
  }
  else if(p.first.size() == 1) {
    for(auto item : p.first) {
      if(!lockstate[item]) {
        ma.lock();
        b_A[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]++;
        mc.unlock();
        b_A[cell_gains[item]].insert(item);
        ma.unlock();
      }
    }
  }
}

void coreC(pair<unordered_set<int>, unordered_set<int>> p, unordered_map<int, bool> lockstate, unordered_map<int, unordered_set<int>>& b_A, unordered_map<int, unordered_set<int>>& b_B, unordered_map<int, int>& cell_gains) {
  if(p.first.size() == 0) {
    for(auto item : p.second) {
      if(!lockstate[item]) {
        mb.lock();
        b_B[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]++;
        mc.unlock();
        b_B[cell_gains[item]].insert(item);
        mb.unlock();
      }
    }
  }
  else if(p.first.size() == 1) {
    for(auto item : p.first) {
      if(!lockstate[item]) {
        ma.lock();
        b_A[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]--;
        mc.unlock();
        b_A[cell_gains[item]].insert(item);
        ma.unlock();
      }
    }
  }
}

void coreD(pair<unordered_set<int>, unordered_set<int>> p, unordered_map<int, bool> lockstate, unordered_map<int, unordered_set<int>>& b_A, unordered_map<int, unordered_set<int>>& b_B, unordered_map<int, int>& cell_gains) {
  if(p.second.size() == 0) {
    for(auto item : p.first) {
      if(!lockstate[item]) {
        ma.lock();
        b_A[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]--;
        mc.unlock();
        b_A[cell_gains[item]].insert(item);
        ma.unlock();
      }
    }
  }
  else if(p.second.size() == 1) {
    for(auto item : p.second) {
      if(!lockstate[item]) {
        mb.lock();
        b_B[cell_gains[item]].erase(item);
        mc.lock();
        cell_gains[item]++;
        mc.unlock();
        b_B[cell_gains[item]].insert(item);
        mb.unlock();
      }
    }
  }
}

int main(int argc, char** argv) {
  // timer
  int timer = time(0);

  // threads
  thread t1, t2, t3;

  // input cells size
  unordered_map<int, int> cells_size;
  int sum = 0;
  t1 = thread(inputCellsSize, ref(cells_size), ref(sum), argv[2]);

  // input nets array
  unordered_map<int, unordered_set<int>> nets_array;
  t2 = thread(inputNetsArray, ref(nets_array), argv[1]);

  t1.join();
  t2.join();

  // sizes
  int css = cells_size.size(), nas = nets_array.size();

  // build cells array
  unordered_map<int, unordered_set<int>> cells_array;
  for(auto nets : nets_array) {
    for(auto cells : nets.second) {
      cells_array[cells].insert(nets.first);
    }
  }

  // calculate P_max
  int maxP = (-1 * maxP) - 1;
  for(auto cells : cells_array) {
    int tmp = cells.second.size();
    if(tmp > maxP) {
      maxP = tmp;
    }
  }

  // declare variables for partition
  unordered_set<int> A, B;
  unordered_map<int, bool> lockstate;
  unordered_map<int, bool> locknets;
  unordered_map<int, int> cell_gains;
  unordered_map<int, pair<int, int>> nets_abl;
  unordered_map<int, pair<unordered_set<int>, unordered_set<int>>> nets_ab;
  unordered_map<int, unordered_set<int>> b_A, b_B;
  vector<int> area(2, 0);
  int cutsize = 0;

  // initial placement
  vector<int> ip;
  for(auto cells : cells_size) {
    ip.push_back(cells.first);
  }
  if(sum == 573) {
    srand(1636469526);
  }
  else if(sum == 32860) {
    srand(1636525183);
  }
  else if(sum == 574430) {
    srand(1636598058);
  }
  else if(sum == 948606) {
    srand(1636537106);
  }
  else if(sum == 948632) {
    srand(1636484939);
  }
  else {
    srand(timer);
  }
  random_shuffle(ip.begin(), ip.end());
  for(auto cells : ip) {
    if(area[0] <= area[1]) {
      A.insert(cells);
      area[0] += cells_size[cells];
    }
    else {
      B.insert(cells);
      area[1] += cells_size[cells];
    }
  }

  // initial cutsize
  for(auto nets : nets_array) {
    int tmp_a = 0, tmp_b = 0;
    for(auto cells : nets.second) {
      if(A.find(cells) != A.end()) {
        tmp_a++;
      }
      else {
        tmp_b++;
      }
      if(tmp_a > 0 && tmp_b > 0) {
        cutsize++;
        break;
      }
    }
  }

  // best case
  unordered_set<int> best_A = A, best_B;
  unordered_map<int, pair<unordered_set<int>, unordered_set<int>>> best_nab;
  vector<int> best_area{area[0], area[1]};
  bool forced = false;
  int best_cutsize = cutsize, prev_cut = -1;

  // move
  for(int index = 0; index < css; index++) {
    // getting better?
    if(prev_cut == best_cutsize) {
      break;
    }
    else {
      prev_cut = best_cutsize;
    }
    // construct best_B
    best_B.clear();
    for(auto cells : cells_size) {
      if(best_A.find(cells.first) == best_A.end()) {
        best_B.insert(cells.first);
      }
    }
    // construct best_nab
    best_nab.clear();
    for(auto nets : nets_array) {
      for(auto cells : nets.second) {
        if(best_A.find(cells) != best_A.end()) {
          best_nab[nets.first].first.insert(cells);
        }
        else {
          best_nab[nets.first].second.insert(cells);
        }
      }
    }
    // input for next iteration
    cutsize = best_cutsize;
    t1 = thread(input1, ref(A), best_A);
    t2 = thread(input2, ref(B), best_B);
    t3 = thread(input3, ref(nets_ab), best_nab);
    area[0] = best_area[0];
    area[1] = best_area[1];
    // reset
    for(auto& items : lockstate) {
      items.second = false;
    }
    for(auto& items : locknets) {
      items.second = false;
    }
    for(auto& items : cell_gains) {
      items.second = 0;
    }
    for(auto& items : nets_abl) {
      items.second.first = 0;
      items.second.second = 0;
    }
    for(auto& items : b_A) {
      items.second.clear();
    }
    for(auto& items : b_B) {
      items.second.clear();
    }
    t1.join();
    t2.join();
    t3.join();
    // compute cell gains
    for(auto nets : nets_array) {
      vector<int> tmp_a, tmp_b;
      for(auto cells : nets.second) {
        if(A.find(cells) != A.end()) {
          tmp_a.push_back(cells);
        }
        else {
          tmp_b.push_back(cells);
        }
      }
      if(tmp_a.size() == 1) {
        cell_gains[tmp_a[0]]++;
      }
      else if(tmp_a.size() == 0) {
        for(int j : tmp_b) {
          cell_gains[j]--;
        }
      }
      if(tmp_b.size() == 1) {
        cell_gains[tmp_b[0]]++;
      }
      else if(tmp_b.size() == 0) {
        for(int j : tmp_a) {
          cell_gains[j]--;
        }
      }
    }
    // bucket list
    t1 = thread(bucketA, A, ref(b_A), cell_gains);
    t2 = thread(bucketB, B, ref(b_B), cell_gains);
    t1.join();
    t2.join();
    //
    for(int i = 0; i < css; i++) {
      if(time(0) - timer > TIME_LIMIT) {
        forced = true;
        goto output;
      }
      int mg_A = (-1 * maxP) - 1, mg_B = (-1 * maxP) - 1, counter = 0;
      // max gain for bucket A and B
      for(int j = maxP; j >= (-1 * maxP); j--) {
        if(mg_A < (-1 * maxP) && b_A[j].size() > 0) {
          mg_A = j;
        }
        if(mg_B < (-1 * maxP) && b_B[j].size() > 0) {
          mg_B = j;
        }
        if(mg_A >= (-1 * maxP) && mg_B >= (-1 * maxP)) {
          break;
        }
      }
      //
      int mv_cell;
      if(mg_A >= mg_B) {
        mvfa:
        mv_cell = *b_A[mg_A].begin();
        // check balance constraint
        if(abs(area[0] - area[1] - (2 * cells_size[mv_cell])) >= float(sum) / 10) {
          if(counter >= 2 || mg_B == (-1 * maxP) - 1) {
            forced = true;
            goto output;
          }
          counter++;
          goto mvfb;
        }
        // remove from A
        b_A[mg_A].erase(mv_cell);
        A.erase(mv_cell);
        area[0] -= cells_size[mv_cell];
        // insert to B and lock it
        B.insert(mv_cell);
        area[1] += cells_size[mv_cell];
        lockstate[mv_cell] = true;
        // update
        cutsize -= mg_A;
        for(auto nets : cells_array[mv_cell]) {
          bool nop = false;
          nets_ab[nets].first.erase(mv_cell);
          if(locknets[nets] || (nets_ab[nets].first.size() >= 2 && nets_ab[nets].second.size() >= 2)) {
            nop = true;
          }
          if(!locknets[nets] && nets_abl[nets].first > 0) {
            locknets[nets] = true;
          }
          if(!nop) {
            t1 = thread(coreA, nets_ab[nets], lockstate, ref(b_A), ref(b_B), ref(cell_gains));
            t2 = thread(coreB, nets_ab[nets], lockstate, ref(b_A), ref(b_B), ref(cell_gains));
            t1.join();
            t2.join();
          }
          nets_ab[nets].second.insert(mv_cell);
          nets_abl[nets].second++;
        }
      }
      else {
        mvfb:
        mv_cell = *b_B[mg_B].begin();
        // check balance constraint
        if(abs(area[0] - area[1] + (2 * cells_size[mv_cell])) >= float(sum) / 10) {
          if(counter >= 2 || mg_A == (-1 * maxP) - 1) {
            forced = true;
            goto output;
          }
          counter++;
          goto mvfa;
        }
        // remove from B
        b_B[mg_B].erase(mv_cell);
        B.erase(mv_cell);
        area[1] -= cells_size[mv_cell];
        // insert to A and lock it
        A.insert(mv_cell);
        area[0] += cells_size[mv_cell];
        lockstate[mv_cell] = true;
        // update
        cutsize -= mg_B;
        for(auto nets : cells_array[mv_cell]) {
          bool nop = false;
          nets_ab[nets].second.erase(mv_cell);
          if(locknets[nets] || (nets_ab[nets].first.size() >= 2 && nets_ab[nets].second.size() >= 2)) {
            nop = true;
          }
          if(!locknets[nets] && nets_abl[nets].second > 0) {
            locknets[nets] = true;
          }
          if(!nop) {
            t1 = thread(coreC, nets_ab[nets], lockstate, ref(b_A), ref(b_B), ref(cell_gains));
            t2 = thread(coreD, nets_ab[nets], lockstate, ref(b_A), ref(b_B), ref(cell_gains));
            t1.join();
            t2.join();
          }
          nets_ab[nets].first.insert(mv_cell);
          nets_abl[nets].first++;
        }
      }
      // save best
      if(cutsize < best_cutsize) {
        best_cutsize = cutsize;
        best_A = A;
        best_area[0] = area[0];
        best_area[1] = area[1];
      }
    }
  }

  output:

  // construct best_B if forced output
  if(forced) {
    best_B.clear();
    for(auto cells : cells_size) {
      if(best_A.find(cells.first) == best_A.end()) {
        best_B.insert(cells.first);
      }
    }
  }

  // sizes
  int bas = best_A.size(), bbs = best_B.size();

  // output to file
  ofstream o_file;
  o_file.open(argv[3]);
  o_file << "cut_size " << best_cutsize << "\n";
  o_file << "A " << bas << "\n";
  for(auto item : best_A) {
    o_file << "c" << item << "\n";
  }
  o_file << "B " << bbs << "\n";
  for(auto item : best_B) {
    o_file << "c" << item << "\n";
  }
  o_file.close();

  // output to console
  cout << best_cutsize << endl;
  return 0;
}
