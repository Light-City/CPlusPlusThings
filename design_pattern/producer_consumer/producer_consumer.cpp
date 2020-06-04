#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <unistd.h>

using namespace std;

const int MAX_NUM = 10000;

class BoundedBuffer
{
public:
  BoundedBuffer(size_t n) {
    array_.resize(n);
    start_pos_ = 0;
    end_pos_ = 0;
    pos_ = 0;
  }
  void Produce(int n) {
    {
      std::unique_lock<std::mutex> lock(mtx_);
      //wait for not full
      not_full_.wait(lock, [=] { return pos_ < array_.size(); });

      usleep(1000 * 400);
      array_[end_pos_] = n;
      end_pos_ = (end_pos_ + 1) % array_.size();
      ++pos_;
      cout << "Produce pos:" << pos_ << endl;
    } //auto unlock

    not_empty_.notify_one();
  }
  
  int Consume() {
    std::unique_lock<std::mutex> lock(mtx_);
    //wait for not empty
    not_empty_.wait(lock, [=] { return pos_ > 0; });
    
    usleep(1000 * 400);
    int n = array_[start_pos_];
    start_pos_ = (start_pos_ + 1) % array_.size();
    --pos_;
    cout << "Consume pos:" << pos_ << endl;
    lock.unlock();

    not_full_.notify_one();

    return n;
  }
private:
  std::vector<int> array_;
  size_t start_pos_;
  size_t end_pos_;
  size_t pos_;
  std::mutex mtx_;
  std::condition_variable not_full_;
  std::condition_variable not_empty_;
};

BoundedBuffer bb(10);
std::mutex g_mtx;

void Producer()
{
  int n = 0;
  while(n < 100) {
    bb.Produce(n);
    cout << "Producer:" << n << endl;
    n++;
  }

  bb.Produce(-1);
}


void Consumer()
{
  std::thread::id thread_id = std::this_thread::get_id();
  int n = 0;
  do {
    n = bb.Consume();
    cout << "Consumer thread:" << thread_id << "=====> " << n << endl;
  } while(-1 != n);

  bb.Produce(-1);
}

int main()
{
  std::vector<std::thread> t;
  t.push_back(std::thread(&Producer));
  t.push_back(std::thread(&Consumer));
  t.push_back(std::thread(&Consumer));
  t.push_back(std::thread(&Consumer));
  
  for (auto& one : t) {
    one.join();
  }

  return 0;
}
