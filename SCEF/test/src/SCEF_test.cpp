//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <filesystem>

#include <SCEF/SCEF.hpp>

/*

#include <CoreLib/Core_OS.hpp>

std::filesystem::path getAppPath()
{
	static std::filesystem::path path = core::applicationPath().parent_path();
	return path;
}
*/

TEST(SCEF, load_sample1)
{
	scef::document doc;

	scef::Error ret = doc.load("sampleFile1.scef", scef::Flag::ForceHeader);

	ASSERT_EQ(ret, scef::Error::None);


	scef::root& root = doc.root();
	ASSERT_EQ(root.size(), 2);

	ASSERT_EQ(root[0]->type(), scef::ItemType::spacer);
	EXPECT_EQ(root[0]->line(), 2);
	EXPECT_EQ(root[0]->column(), 0);

	ASSERT_EQ(root[1]->type(), scef::ItemType::group);
	EXPECT_EQ(root[1]->line(), 3);
	EXPECT_EQ(root[1]->column(), 0);

	{
		scef::group& l1_group = *static_cast<scef::group*>(root[1].get());
		EXPECT_EQ(l1_group.quotation_mode(), scef::QuotationMode::standard);
		ASSERT_EQ(l1_group.name(), U"Sample");
		ASSERT_EQ(l1_group.size(), 8);

		//spacers
		ASSERT_EQ(l1_group[0]->type(), scef::ItemType::spacer);
		EXPECT_EQ(l1_group[0]->line(), 3);
		EXPECT_EQ(l1_group[0]->column(), 8);

		ASSERT_EQ(l1_group[2]->type(), scef::ItemType::spacer);
		EXPECT_EQ(l1_group[2]->line(), 4);
		EXPECT_EQ(l1_group[2]->column(), 7);

		ASSERT_EQ(l1_group[4]->type(), scef::ItemType::spacer);
		EXPECT_EQ(l1_group[4]->line(), 5);
		EXPECT_EQ(l1_group[4]->column(), 13);

		ASSERT_EQ(l1_group[6]->type(), scef::ItemType::spacer);
		EXPECT_EQ(l1_group[6]->line(), 11);
		EXPECT_EQ(l1_group[6]->column(), 2);

		//comment
		ASSERT_EQ(l1_group[7]->type(), scef::ItemType::comment);
		EXPECT_EQ(l1_group[7]->line(), 13);
		EXPECT_EQ(l1_group[7]->column(), 1);

		//value
		ASSERT_EQ(l1_group[1]->type(), scef::ItemType::value);
		{
			scef::value& tvalue = *static_cast<scef::value*>(l1_group[1].get());
			ASSERT_EQ(tvalue.name(), U"value");
			EXPECT_EQ(tvalue.quotation_mode(), scef::QuotationMode::standard);
			EXPECT_EQ(tvalue.line(), 4);
			EXPECT_EQ(tvalue.column(), 1);
		}

		//key
		ASSERT_EQ(l1_group[3]->type(), scef::ItemType::key_value);
		{
			scef::keyedValue& tkey = *static_cast<scef::keyedValue*>(l1_group[3].get());
			ASSERT_EQ(tkey.name(), U"key");
			ASSERT_EQ(tkey.value(), U"value");
			EXPECT_EQ(tkey.quotation_mode(), scef::QuotationMode::standard);
			EXPECT_EQ(tkey.line(), 5);
			EXPECT_EQ(tkey.column(), 1);

			EXPECT_EQ(tkey.value_quotation_mode(), scef::QuotationMode::standard);
			EXPECT_EQ(tkey.column_value(), 7);
		}

		//group
		ASSERT_EQ(l1_group[5]->type(), scef::ItemType::group);
		{
			scef::group& l2_group = *static_cast<scef::group*>(l1_group[5].get());
			EXPECT_EQ(l2_group.quotation_mode(), scef::QuotationMode::singlemark);
			ASSERT_EQ(l2_group.name(), U"Nested With Escape");
			ASSERT_EQ(l2_group.size(), 7);

			//spacers
			ASSERT_EQ(l2_group[0]->type(), scef::ItemType::spacer);
			EXPECT_EQ(l2_group[0]->line(), 7);
			EXPECT_EQ(l2_group[0]->column(), 23);

			ASSERT_EQ(l2_group[2]->type(), scef::ItemType::spacer);
			EXPECT_EQ(l2_group[2]->line(), 8);
			EXPECT_EQ(l2_group[2]->column(), 32);

			ASSERT_EQ(l2_group[4]->type(), scef::ItemType::spacer);
			EXPECT_EQ(l2_group[4]->line(), 9);
			EXPECT_EQ(l2_group[4]->column(), 17);

			ASSERT_EQ(l2_group[6]->type(), scef::ItemType::spacer);
			EXPECT_EQ(l2_group[6]->line(), 10);
			EXPECT_EQ(l2_group[6]->column(), 28);

			//value
			ASSERT_EQ(l2_group[3]->type(), scef::ItemType::value);
			{
				scef::value& tvalue = *static_cast<scef::value*>(l2_group[3].get());
				ASSERT_EQ(tvalue.name(), U"Escape value");
				EXPECT_EQ(tvalue.quotation_mode(), scef::QuotationMode::singlemark);
				EXPECT_EQ(tvalue.line(), 9);
				EXPECT_EQ(tvalue.column(), 2);
			}

			//key
			ASSERT_EQ(l2_group[1]->type(), scef::ItemType::key_value);
			{
				scef::keyedValue& tkey = *static_cast<scef::keyedValue*>(l2_group[1].get());
				ASSERT_EQ(tkey.name(), U"Escape Key");
				ASSERT_EQ(tkey.value(), U"Escape Value");
				EXPECT_EQ(tkey.quotation_mode(), scef::QuotationMode::singlemark);
				EXPECT_EQ(tkey.line(), 8);
				EXPECT_EQ(tkey.column(), 2);

				EXPECT_EQ(tkey.value_quotation_mode(), scef::QuotationMode::doublemark);
				EXPECT_EQ(tkey.column_value(), 17);
			}

			//value with escape sequences
			ASSERT_EQ(l2_group[5]->type(), scef::ItemType::value);
			{
				scef::value& tvalue = *static_cast<scef::value*>(l2_group[5].get());
				EXPECT_EQ(tvalue.quotation_mode(), scef::QuotationMode::singlemark);
				EXPECT_EQ(tvalue.line(), 10);
				EXPECT_EQ(tvalue.column(), 2);

				const std::u32string& text = tvalue.name();
				ASSERT_EQ(text.size(), 5);
				EXPECT_EQ(text[0], U'\n');
				EXPECT_EQ(text[1], U'^');
				EXPECT_EQ(text[2], char32_t{0x23});
				EXPECT_EQ(text[3], char32_t{0x1234});
				EXPECT_EQ(text[4], char32_t{0x12345678});
			}
		}
	}

}
