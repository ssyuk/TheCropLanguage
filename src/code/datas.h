#ifndef CROP_DATA_H
#define CROP_DATA_H

#include <any>
#include <string>
#include <utility>
#include <vector>

using namespace std;

enum VariableType {
    VOID, INT, FLOAT, CHAR, BOOL, OBJECT
};

class Type {
public:
    explicit Type() : type(VOID) {}

    explicit Type(VariableType type) : type(type) {}

    explicit Type(string objName) : objName(std::move(objName)) { type = OBJECT; }

    VariableType type;
    string objName;

    bool operator==(const Type &other) const {
        return type == other.type;
    }

    bool operator!=(const Type &other) const {
        return type != other.type;
    }
};

typedef struct {
    vector<string> name;
} Parameter;

typedef struct {
    Type type;
    any value;
} Variable;

enum CodeType {
    EXECUTE_FUNCTION,
    CREATE_VARIABLE,
    UPDATE_VARIABLE,
    IF_STATEMENT,
};

class Code {
public:
    Code(string functionName, const vector<any> &functionArguments)
            : functionName(std::move(functionName)),
              functionArguments(functionArguments) {
        type = EXECUTE_FUNCTION;
    }

    Code(Type variableType, string variableName, any variableData)
            : variableType(std::move(variableType)),
              variableName(std::move(variableName)),
              variableData(std::move(variableData)) {
        type = CREATE_VARIABLE;
    }

    Code(string variableName, any variableData)
            : variableName(std::move(variableName)),
              variableData(std::move(variableData)) {
        type = UPDATE_VARIABLE;
    }

    Code(const string &statement, const vector<Code> &ifCodeBlock, const vector<Code> &elseCodeBlock = vector<Code>())
            : statement(
            statement), ifCodeBlock(ifCodeBlock), elseCodeBlock(elseCodeBlock) {
        type = IF_STATEMENT;
    }

    CodeType type;
    string functionName;
    vector<any> functionArguments;
    Type variableType;
    string variableName;
    any variableData;
    string statement;
    vector<Code> ifCodeBlock;
    vector<Code> elseCodeBlock;
};

class Function {
public:
    Function(Type returnType, string name, const vector<Code> &codes)
            : returnType(std::move(returnType)), name(std::move(name)), codes(codes) {
    }

    Type returnType;
    string name;
    vector<Code> codes;
};

class DotCrop {
public:
    DotCrop(string aPackage, const vector<string> &imports,
            const vector<Function> &functions)
            : package(std::move(aPackage)), imports(imports), functions(functions) {}

    string package;
    vector<string> imports;
    vector<Function> functions;
};

class CropProject {
public:
    CropProject(DotCrop mainFile, const vector<DotCrop> &files)
            : mainFile(std::move(mainFile)), files(files) {}

    DotCrop mainFile;
    vector<DotCrop> files;
};

typedef struct {
    string mainFile;
    vector<string> files;
    string addonPath;
} ProjectConfig;

#endif // CROP_DATA_H