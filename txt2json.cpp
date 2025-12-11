#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::string trim(const std::string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

std::string escape_json(const std::string &s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if ((unsigned char)c < 0x20) {
                    char buf[8];
                    sprintf(buf, "\\u%04x", (unsigned char)c);
                    out += buf;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

struct Article {
    std::string title;
    std::vector<std::string> authors;
    std::string body;
};

class ArticleBuilder {
public:
    virtual ~ArticleBuilder() = default;
    virtual void setTitle(const std::string& t) = 0;
    virtual void setAuthors(const std::vector<std::string>& a) = 0;
    virtual void setBody(const std::string& b) = 0;
    virtual std::string build() const = 0;
};

class ArticleJsonBuilder : public ArticleBuilder {
    Article article;
public:
    void setTitle(const std::string& t) override { 
        article.title = t; 
    }
    void setAuthors(const std::vector<std::string>& a) override { 
        article.authors = a; 
    }
    void setBody(const std::string& b) override { 
        article.body = b; 
    }

    std::string build() const override {
        std::ostringstream out;
        out << "{\n";
        out << "  \"title\": \"" << escape_json(article.title) << "\",\n";
        out << "  \"authors\": [";
        for (size_t i = 0; i < article.authors.size(); i++) {
            out << "\"" << escape_json(article.authors[i]) << "\"";
            if (i + 1 < article.authors.size()) out << ", ";
        }
        out << "],\n";
        out << "  \"body\": \"" << escape_json(article.body) << "\"\n";
        out << "}";
        return out.str();
    }
};

class ArticleDirector {
public:
    void construct(ArticleBuilder& builder,
                   const std::string& title,
                   const std::vector<std::string>& authors,
                   const std::string& body) {
        builder.setTitle(title);
        builder.setAuthors(authors);
        builder.setBody(body);
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.txt> [output.json]\n";
        return 1;
    }
    std::string inpath = argv[1];
    std::string outpath = (argc >= 3) ? argv[2] : "output.json";
    std::ifstream fin(inpath);
    if (!fin) {
        std::cerr << "Ошибка при открытии: " << inpath << "\n";
        return 2;
    }
    std::vector<std::string> lines;
    std::string line;
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();
    if (lines.size() < 2) {
        std::cerr << "В файле нужен хотя бы заголовок и авторы.\n";
        return 3;
    }
    std::string title = trim(lines[0]);
    std::string authors_line = trim(lines[1]);
    std::vector<std::string> authors;
    std::stringstream ss(authors_line);
    std::string part;
    while (getline(ss, part, ',')) {
        authors.push_back(trim(part));
    }
    std::string body;
    for (size_t i = 2; i < lines.size(); i++) {
        body += lines[i];
        if (i != lines.size() - 1) body += "\n";
    }
    ArticleJsonBuilder builder;
    ArticleDirector director;
    director.construct(builder, title, authors, body);
    std::string json = builder.build();
    std::ofstream fout(outpath);
    if (!fout) {
        std::cerr << "Ошибка при записи: " << outpath << "\n";
        return 4;
    }
    fout << json;
    fout.close();
    std::cout << "Конвертация завершена: " << outpath << "\n";
    return 0;
}