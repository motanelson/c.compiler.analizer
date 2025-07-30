open System
open System.IO
open System.Text

[<EntryPoint>]
let main _ =
    Console.Clear()
    Console.BackgroundColor <- ConsoleColor.DarkYellow
    Console.ForegroundColor <- ConsoleColor.Black
    Console.WriteLine("\nCriando ficheiro main.wasm...\n")

    use fs = new FileStream("main.wasm", FileMode.Create, FileAccess.Write)
    use writer = new BinaryWriter(fs)

    // Cabeçalho
    writer.Write([| 0x00uy; byte 'a'; byte 's'; byte 'm' |])
    writer.Write([| 0x01uy; 0x00uy; 0x00uy; 0x00uy |])

    // Seção de tipos
    writer.Write(0x01uy)       // section id: type
    writer.Write(0x07uy)       // section size
    writer.Write(0x01uy)       // number of types
    writer.Write(0x60uy)       // func type
    writer.Write(0x02uy)       // param count
    writer.Write(0x7Fuy)       // i32
    writer.Write(0x7Fuy)       // i32
    writer.Write(0x01uy)       // result count
    writer.Write(0x7Fuy)       // i32

    // Seção de funções
    writer.Write(0x03uy)       // section id: function
    writer.Write(0x02uy)       // section size
    writer.Write(0x01uy)       // function count
    writer.Write(0x00uy)       // type index

    // Seção de exportações
    writer.Write(0x07uy)       // section id: export
    writer.Write(0x07uy)       // section size
    writer.Write(0x01uy)       // export count
    writer.Write(0x03uy)       // string length
    writer.Write(Encoding.ASCII.GetBytes("sum"))
    writer.Write(0x00uy)       // export kind: function
    writer.Write(0x00uy)       // function index

    // Seção de código
    writer.Write(0x0Auy)       // section id: code
    writer.Write(0x09uy)       // section size
    writer.Write(0x01uy)       // function count
    writer.Write(0x07uy)       // body size
    writer.Write(0x00uy)       // local decl count

    // Código: local.get 0, local.get 1, i32.add, end
    let code = [| 0x20uy; 0x00uy; 0x20uy; 0x01uy; 0x6Auy; 0x0Buy |]
    writer.Write(code)

    Console.WriteLine("Ficheiro main.wasm criado com sucesso!")
    0
