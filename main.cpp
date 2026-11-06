#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
constexpr double pi=3.14159265358979323846;
struct Patch{double frequency=220,duration=3,cutoff=1600,echo=.2;std::string wave="sine";};
double oscillator(double phase,const std::string& wave){
 phase-=std::floor(phase);if(wave=="square")return phase<.5?1:-1;
 if(wave=="saw")return 2*phase-1;if(wave=="triangle")return 1-4*std::abs(phase-.5);
 return std::sin(2*pi*phase);
}
std::vector<double> render(const Patch& p){
 const int rate=44100;std::vector<double> out(static_cast<size_t>(p.duration*rate));
 const double alpha=1-std::exp(-2*pi*p.cutoff/rate);double filtered=0;
 for(size_t i=0;i<out.size();++i){double t=double(i)/rate;
  double envelope=std::min(1.,t/.03)*std::min(1.,(p.duration-t)/.25);
  filtered+=alpha*(oscillator(t*p.frequency,p.wave)*envelope-filtered);
  double value=filtered*.65;if(i>=11025)value+=out[i-11025]*p.echo;
  out[i]=std::clamp(value,-1.,1.);
 }return out;
}
void word(std::ostream& out,uint32_t value,int bytes){for(int i=0;i<bytes;i++)out.put(char((value>>(i*8))&255));}
void wav(const std::string& name,const std::vector<double>& samples){
 std::ofstream out(name,std::ios::binary);if(!out)throw std::runtime_error("Cannot open output file");
 out.write("RIFF",4);word(out,36+uint32_t(samples.size()*2),4);out.write("WAVEfmt ",8);word(out,16,4);word(out,1,2);word(out,1,2);word(out,44100,4);word(out,88200,4);word(out,2,2);word(out,16,2);out.write("data",4);word(out,uint32_t(samples.size()*2),4);
 for(double value:samples)word(out,uint16_t(int16_t(value*32767)),2);
 if(!out)throw std::runtime_error("Failed writing WAV");
}
bool valid(const Patch& p){return std::isfinite(p.frequency)&&std::isfinite(p.duration)&&std::isfinite(p.cutoff)&&std::isfinite(p.echo)&&p.frequency>=20&&p.frequency<=10000&&p.duration>=.1&&p.duration<=60&&p.cutoff>=20&&p.cutoff<=20000&&p.echo>=0&&p.echo<=.8&&(p.wave=="sine"||p.wave=="square"||p.wave=="saw"||p.wave=="triangle");}
int main(int argc,char**argv){try{
 if(argc==2&&std::string(argv[1])=="--self-test"){
  Patch p;p.duration=.1;auto s=render(p);if(s.size()!=4410)return 1;double peak=0;for(auto v:s){if(!std::isfinite(v)||std::abs(v)>1)return 1;peak=std::max(peak,std::abs(v));}return peak>.1?0:1;
 }
 Patch p;std::string command;std::cout<<"PATCH SYNTH\noscillator -> envelope -> low-pass -> echo -> WAV\nCommands: wave sine|square|saw|triangle, freq Hz, duration seconds, cutoff Hz, echo 0..0.8, render file.wav, show, quit\n";
 while(std::cout<<"> "&&std::cin>>command){
  if(command=="quit")break;if(command=="render"){std::string file;std::cin>>file;wav(file,render(p));std::cout<<"Rendered "<<file<<"\n";continue;}
  if(command=="show"){std::cout<<p.wave<<" "<<p.frequency<<"Hz -> low-pass "<<p.cutoff<<"Hz -> echo "<<p.echo<<" | "<<p.duration<<"s\n";continue;}
  Patch next=p;if(command=="wave")std::cin>>next.wave;else if(command=="freq")std::cin>>next.frequency;else if(command=="duration")std::cin>>next.duration;else if(command=="cutoff")std::cin>>next.cutoff;else if(command=="echo")std::cin>>next.echo;else{std::cout<<"Unknown command\n";std::string rest;std::getline(std::cin,rest);continue;}
  if(!std::cin){std::cin.clear();std::string rest;std::getline(std::cin,rest);std::cout<<"Expected a value\n";continue;}
  if(valid(next))p=next;else std::cout<<"Value outside supported range\n";
 }
 return 0;
 }catch(const std::exception&e){std::cerr<<e.what()<<"\n";return 1;}}
