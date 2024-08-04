
class DataFormat {}

class AST<dag tree, dag tag> {
  dag TreeData = tree;
  dag TagData  = tag;
}

def ins;

def Vec : DataFormat;
def Optional : DataFormat;


def Id : AST< (ins "id":$string), (ins) >;
