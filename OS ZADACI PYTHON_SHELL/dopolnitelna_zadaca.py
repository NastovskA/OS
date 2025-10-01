import sys

if len(sys.argv) != 2:
    print("Koristenje: python script.py vlezna_datoteka.xml")
    sys.exit(1)

# Вчитување на влезната датотека
input_file = sys.argv[1]
output_file = input_file.rsplit(".", 1)[0] + ".html"

# Читање на влезната датотека
f = open(input_file, "r")
xml_content = f.read()
f.close()

# Заменуваме \n со <br/>
xml_content = xml_content.replace("\n", "<br/>")

# Наоѓаме сите <Book> тагови и ги заменуваме со HTML формат
books = set(xml_content.split("<Book>"))  # користење на set за уникатни книги
formatted_books = []
book_count = 0

for book in books:
    if "</Book>" in book:
        book_content, rest = book.split("</Book>", 1)
        book_html = f"<div class='book'><h2>{book_content}</h2></div>"
        formatted_books.append(book_html)
        book_count += 1

# Запишување во HTML излезна датотека
f = open(output_file, "w", encoding="utf-8")
f.write("<html><body>\n")
f.write("\n".join(formatted_books))
f.write("\n</body></html>")
f.close()

print(f"Pronajdeni {book_count} knigi. Rezultatot e zapisan vo '{output_file}'.")
