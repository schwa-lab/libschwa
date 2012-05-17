namespace schwa {
  namespace msgpack {

    class Object;

    class ArrayObject {
    public:
      uint32_t size;
      Object *items;
    };

    class MapObject {
    public:
      uint32_t size;
      Object *keys;
      Object *vals;
    };

    class RawObject {
    public:
      uint32_t size;
      char *data;
    };

    enum ObjectType {
      TYPE_NIL,
      TYPE_BOOLEAN,
      TYPE_UINT,
      TYPE_INT,
      TYPE_FLOAT,
      TYPE_DOUBLE,
      TYPE_RAW,
      TYPE_ARRAY,
      TYPE_MAP
    };

    union ObjectValue {
      bool _bool;
      uint64_t _uint;
      int64_t _int;
      float _float;
      double _double;
      ArrayObject _array;
      MapObject _map;
      RawObject _raw;
    };

    class Object {
    public:
      ObjectType type;
      ObjectValue via;
    };

  }
}
