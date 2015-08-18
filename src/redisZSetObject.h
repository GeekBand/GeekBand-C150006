#ifndef _REDISZSETOBJECT_H_
#define _REDISZSETOBJECT_H_


#include <string>
#include <boost/shared_ptr.hpp>

#include "redisObject.h"

namespace redis
{

class ZSetObject : public Object
{
public:
  const std::string& typeName() const { return typeName_; }

  //zadd，新增元素和分值，返回是否正确，可以批量添加
  bool zadd(std::map<double, StrObjectPtr&>& map);
  bool zadd(std::vector<std::map<double, StrObjectPtr&>&>& mapList);

  //zcard，获取dict_的全部元素数量
  int zcard();

  //zscore，获取某个对象的分值，返回分值
  double zscore(StrObjectPtr& obj);

  //zcount，或者两个分值之间的元素数量
  //这个是按分值来筛选，(0, -1)也可以筛选全部
  int zcount(double scoreLeft, double scoreRight);
  //zincrby
  double zincrby(double incrby, StrObjectPtr& obj);

  //zrange，返回两个下标之间的元素，可以控制是否显示分值
  //这个是按跳跃表的下标来筛选
  std::map<StrObjectPtr&, double>& zrange(int leftIndex, int rightIndex, bool withScores);
  //zrangebyscore
  std::map<StrObjectPtr&, double>& zrangebyscore(double scoreLeft, int scoreRight, bool withScores);

  //zrank，返回obj的排名
  int zrank(StrObjectPtr& obj);

  //zrem，删除元素，也可以批量删除
  bool zrem(StrObjectPtr& obj);
  bool zrem(std::vector<StrObjectPtr&> objList);

private:
  //map，按照分值进行排序，获取字符串对象还需要
  std::map<double, StrObjectPtr&> dict_;

  static std::string typeName_;
};

typedef boost::shared_ptr<ZSetObject>  ZSetObjectPtr;

}

#endif
