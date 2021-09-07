#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QSettings>
#include <memory>

enum class ProjectType {
    GUI,
    Console,
    StaticLib,
    DynamicLib
};

class Project;
class Editor;
class CppParser;

struct FolderNode;
using PFolderNode = std::shared_ptr<FolderNode>;
struct FolderNode {
    QString text;
    FolderNode * parent;
    int unitIndex;
    QList<PFolderNode>  children;
};

class ProjectUnit {

public:
    explicit ProjectUnit(Project* parent);
    Project* parent() const;
    void setParent(Project* newParent);
    Editor *editor() const;
    void setEditor(Editor *newEditor);
    const QString &fileName() const;
    void setFileName(const QString &newFileName);
    bool isNew() const;
    void setNew(bool newNew);
    const QString &folder() const;
    void setFolder(const QString &newFolder);
    bool compile() const;
    void setCompile(bool newCompile);
    bool compileCpp() const;
    void setCompileCpp(bool newCompileCpp);
    bool overrideBuildCmd() const;
    void setOverrideBuildCmd(bool newOverrideBuildCmd);
    const QString &buildCmd() const;
    void setBuildCmd(const QString &newBuildCmd);
    bool link() const;
    void setLink(bool newLink);
    int priority() const;
    void setPriority(int newPriority);
    bool detectEncoding() const;
    void setDetectEncoding(bool newDetectEncoding);
    const QByteArray &encoding() const;
    void setEncoding(const QByteArray &newEncoding);
    bool modified() const;
    void setModified(bool value);
    bool save();

    PFolderNode &node();
    void setNode(const PFolderNode &newNode);

private:
    Project* mParent;
    Editor* mEditor;
    QString mFileName;
    bool mNew;
    QString mFolder;
    bool mCompile;
    bool mCompileCpp;
    bool mOverrideBuildCmd;
    QString mBuildCmd;
    bool mLink;
    int mPriority;
    bool mDetectEncoding;
    QByteArray mEncoding;
    PFolderNode mNode;
};

using PProjectUnit = std::shared_ptr<ProjectUnit>;

struct ProjectVersionInfo{
    int major;
    int minor;
    int release;
    int build;
    int languageID;
    int charsetID;
    QString companyName;
    QString fileVersion;
    QString fileDescription;
    QString internalName;
    QString legalCopyright;
    QString legalTrademarks;
    QString originalFilename;
    QString productName;
    QString productVersion;
    bool autoIncBuildNr;
    bool syncProduct;
};

struct ProjectOptions{
    ProjectType type;
    int version;
    bool useUTF8;
    QStringList objfiles;
    QString compilerCmd;
    QString cppCompilerCmd;
    QString linkerCmd;
    QStringList includes;
    QStringList libs;
    QString privateResource;
    QStringList resourceIncludes;
    QStringList makeIncludes;
    bool useGPP;
    QString icon;
    QString exeOutput;
    QString objectOutput;
    QString logOutput;
    bool logOutputEnabled;
    bool useCustomMakefile;
    QString customMakefile;
    bool usePrecompiledHeader;
    bool precompiledHeader;
    bool overrideOutput;
    QString overridenOutput;
    QString hostApplication;
    bool includeVersionInfo;
    bool supportXPThemes;
    int compilerSet;
    QString compilerOptions;
    ProjectVersionInfo VersionInfo;
    QString cmdLineArgs;
    bool mStaticLink;
    bool mAddCharset;
};

class Project : public QObject
{
    Q_OBJECT
public:
    explicit Project(QObject *parent = nullptr);
    QString directory();
    QString executable();
    QString makeFileName();
    bool modified();
    void setFileName(const QString& value);
    void setModified(bool value);

    void addFolder(const QString& s);
    PProjectUnit addUnit(const QString& inFileName,
                PFolderNode parentNode,
                bool rebuild);
    void buildPrivateResource(bool forceSave);

    int  newUnit(bool newProject,
                 PFolderNode parentNode,
                 const QString customFileName);
    QString getFolderPath(PFolderNode node);
    void updateFolders();
    Editor* openUnit(int index);
    void closeUnit(int index);
    void doAutoOpen();
    void saveUnitAs(int i, const QString& sFileName); // save single [UnitX]
    void saveAll(); // save [Project] and  all [UnitX]
    void loadLayout(); // load all [UnitX]
    void loadUnitLayout(Editor *e, int index); // load single [UnitX] cursor positions
    void saveLayout(); // save all [UnitX]
    void saveUnitLayout(Editor* e, int index); // save single [UnitX] cursor positions
    PFolderNode makeProjectNode();
    PFolderNode makeNewFileNode(const QString& s, bool isFolder, PFolderNode newParent);
    void loadOptions();
    void saveOptions();
    bool saveUnits();
//    procedure Open;
    bool fileAlreadyExists(const QString& s);
    bool removeFolder(PFolderNode node);
    bool removeEditor(int index, bool doClose);
    int getUnitFromString(const QString& s);
    void rebuildNodes();
    QString listUnitStr(const QChar& separator);
    void exportToHTML();
    void showOptions();
    // bool assignTemplate(const QString& aFileName, const PTemplate& aTemplate);
    PFolderNode folderNodeFromName(const QString& name);
    void createFolderNodes();
    void updateNodeIndexes();
    void setNodeValue(PFolderNode value);
    void checkProjectFileForUpdate();
    void incrementBuildNumber();
    QChar getCompilerOption(const QString& optionString);
    void setCompilerOption(const QString& optionString, const QChar& value);
    void saveToLog();
signals:
    void nodesChanged();
    void modifyChanged(bool value);
private:
    void open();
    void sortUnitsByPriority();
private:
    QList<PProjectUnit> mUnits;
    ProjectOptions mOptions;
    std::shared_ptr<QSettings> mIniFile;
    QString mFilename;
    QString mName;
    bool mModified;
    QStringList mFolders;
    std::shared_ptr<CppParser> mParser;
    PFolderNode mNode;
};

#endif // PROJECT_H
